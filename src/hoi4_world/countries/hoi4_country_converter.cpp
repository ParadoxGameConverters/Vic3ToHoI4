#include "src/hoi4_world/countries/hoi4_country_converter.h"

#include <numeric>
#include <ranges>
#include <vector>

#include "external/fmt/include/fmt/format.h"
#include "src/hoi4_world/characters/hoi4_character_converter.h"
#include "src/hoi4_world/characters/hoi4_characters_converter.h"
#include "src/hoi4_world/military/task_force_template.h"
#include "src/hoi4_world/states/hoi4_state.h"
#include "src/hoi4_world/technology/technologies_converter.h"
#include "src/mappers/character/leader_type_mapper.h"
#include "src/support/converter_utils.h"
#include "src/vic3_world/countries/vic3_country.h"
#include "src/vic3_world/ideologies/ideologies.h"
#include "src/vic3_world/world/vic3_world.h"

namespace
{

constexpr float tolerance = 0.01F;

bool StateAsCapitalCompareFunction(const hoi4::State& a, const hoi4::State& b)
{
   // More victory points has priority.
   const auto& a_victory_points = a.GetVictoryPoints();
   const auto& b_victory_points = b.GetVictoryPoints();
   const int a_victory_points_total = std::accumulate(a_victory_points.begin(),
       a_victory_points.end(),
       0,
       [](int total, const std::pair<int, int>& victory_point) {
          return victory_point.second + total;
       });
   const int b_victory_points_total = std::accumulate(b_victory_points.begin(),
       b_victory_points.end(),
       0,
       [](int total, const std::pair<int, int>& victory_point) {
          return victory_point.second + total;
       });
   if (a_victory_points_total > b_victory_points_total)
   {
      return true;
   }
   if (a_victory_points_total < b_victory_points_total)
   {
      return false;
   }

   // Next, higher industry matters.
   const int a_factories = a.GetCivilianFactories() + a.GetMilitaryFactories() + a.GetDockyards();
   const int b_factories = b.GetCivilianFactories() + b.GetMilitaryFactories() + b.GetDockyards();
   if (a_factories > b_factories)
   {
      return true;
   }
   if (a_factories < b_factories)
   {
      return false;
   }

   // Still here? Try population.
   if (a.GetManpower() > b.GetManpower())
   {
      return true;
   }
   if (a.GetManpower() < b.GetManpower())
   {
      return false;
   }

   // There's nothing else left. Lowest id wins.
   return a.GetId() < b.GetId();
}


std::optional<int> DetermineBackupCapital(const std::string_view& tag, const std::vector<hoi4::State>& states)
{
   std::vector<std::reference_wrapper<const hoi4::State>> owned_states;
   for (const hoi4::State& state: states)
   {
      if (state.GetOwner() != tag)
      {
         continue;
      }

      owned_states.emplace_back(state);
   }

   if (owned_states.empty())
   {
      return std::nullopt;
   }
   std::ranges::sort(owned_states, StateAsCapitalCompareFunction);

   return owned_states.begin()->get().GetId();
}


std::optional<int> ConvertCapital(const vic3::Country& source_country,
    std::string_view tag,
    const std::map<int, int>& vic3_state_ids_to_hoi4_state_ids,
    const std::vector<hoi4::State>& states)
{
   if (const std::optional<int> vic3_capital_state = source_country.GetCapitalState(); vic3_capital_state)
   {
      if (const auto state_id_mapping = vic3_state_ids_to_hoi4_state_ids.find(*vic3_capital_state);
          state_id_mapping != vic3_state_ids_to_hoi4_state_ids.end())
      {
         return state_id_mapping->second;
      }
   }

   return DetermineBackupCapital(tag, states);
}

date ConvertElection(const std::optional<date>& vic_election)
{
   const auto start_date = date("1936.1.1");
   constexpr int election_period = 4;  // All Vic elections have 4-year cycles
   const auto pivot_date = date(start_date.getYear() - election_period, start_date.getMonth(), start_date.getDay());

   if (!vic_election)  // Country has no elections in Vic
   {
      return date(pivot_date.getYear() + 1, start_date.getMonth(), start_date.getDay());
   }

   date last_election = vic_election.value();
   int election_year = pivot_date.getYear();
   if (const auto year_offset = FloorMod(pivot_date.getYear() - last_election.getYear(), election_period);
       year_offset == 0)
   {
      // Only matters when last_election is on January 1st.
      // Or if we ever allow non January 1st start dates.
      if (pivot_date >= date(pivot_date.getYear(), last_election.getMonth(), last_election.getDay()))
      {
         election_year = pivot_date.getYear() + election_period;
      }
   }
   else
   {
      election_year = pivot_date.getYear() + election_period - year_offset;
   }

   last_election = date(election_year, last_election.getMonth(), last_election.getDay());
   return last_election;
}


bool ShouldHaveMonarchIdea(bool has_elections, const std::set<std::string>& active_laws)
{
   return has_elections && active_laws.contains("law_monarchy");
}


std::vector<hoi4::EquipmentVariant> DetermineActiveVariants(const std::vector<hoi4::EquipmentVariant>& all_variants,
    const hoi4::Technologies& technologies)
{
   std::vector<hoi4::EquipmentVariant> active_variants;

   for (const hoi4::EquipmentVariant& variant: all_variants)
   {
      if (!std::ranges::all_of(variant.GetRequiredTechs(), [technologies](const std::string& required_technology) {
             return technologies.HasTechnology(required_technology);
          }))
      {
         continue;
      }
      if (std::ranges::any_of(variant.GetBlockingTechs(), [technologies](const std::string& blocking_technology) {
             return technologies.HasTechnology(blocking_technology);
          }))
      {
         continue;
      }

      active_variants.push_back(variant);
   }

   return active_variants;
}


std::optional<hoi4::Unit> FillTemplate(const hoi4::DivisionTemplate& division,
    std::vector<hoi4::Battalion>& battalions,
    int default_location)
{
   hoi4::UnitCount required = division.GetBattalions();
   for (const auto& [ut, str]: division.GetSupport())
   {
      required[ut] += str;
   }
   float total = 0.0F;
   for (const auto& [ut, str]: required)
   {
      total += str;
      float found = 0.0F;
      for (const auto& battalion: battalions)
      {
         if (battalion.GetType() != ut)
         {
            continue;
         }
         found += battalion.GetStrength();
      }
      if (found < str)
      {
         return {};
      }
   }
   if (total < tolerance)
   {
      // Avoid an infinite loop of making divisions that don't require any strength.
      return {};
   }
   int equipment = 100;
   int location = default_location;
   for (const auto& [ut, str]: required)
   {
      float needed = str;
      for (auto& battalion: battalions)
      {
         if (battalion.GetType() != ut)
         {
            continue;
         }
         auto current = battalion.GetStrength();
         if (current > needed)
         {
            current = needed;
         }
         battalion.AddStrength(-current);
         needed -= current;
         // Create division with equipment of worst battalion in it.
         // Approximate but reasonable.
         equipment = std::min(equipment, std::min(100, battalion.GetEquipmentScale()));
         const auto& battalion_location = battalion.GetLocation();
         if (battalion_location.has_value())
         {
            location = *battalion_location;
         }
         if (needed < tolerance)
         {
            break;
         }
      }
   }

   std::erase_if(battalions, [](const hoi4::Battalion& cand) {
      return cand.GetStrength() < tolerance;
   });

   return hoi4::Unit{division.GetName(), equipment, location};
}

void extractActiveItems(const std::vector<hoi4::EquipmentVariant>& variants, std::set<std::string>& active)
{
   const std::set<std::string> toExtract{"name", "type"};
   for (const auto& variant: variants)
   {
      const auto& items = variant.GetTextItems();
      for (const auto& item: items)
      {
         if (!toExtract.contains(item.first))
         {
            continue;
         }
         active.insert(commonItems::remQuotes(item.second));
      }
   }
}

std::vector<hoi4::TaskForce> ConvertNavies(const std::string& tag,
    const vic3::Buildings& buildings,
    const std::vector<hoi4::TaskForceTemplate>& task_force_templates,
    const std::vector<hoi4::EquipmentVariant>& active_ship_variants,
    const std::vector<hoi4::EquipmentVariant>& active_legacy_ship_variants,
    const hoi4::States& states)
{
   std::vector<hoi4::TaskForce> forces;
   std::map<std::string, float> pm_amounts;
   std::map<std::string, int> ship_names;
   std::set<std::string> active_variants;
   extractActiveItems(active_ship_variants, active_variants);
   extractActiveItems(active_legacy_ship_variants, active_variants);

   for (const auto& [vic3_id, hoi4_id]: states.vic3_state_ids_to_hoi4_state_ids)
   {
      const auto itr = states.hoi4_state_ids_to_owner.find(hoi4_id);
      if (itr == states.hoi4_state_ids_to_owner.end())
      {
         continue;
      }
      if (itr->second != tag)
      {
         continue;
      }
      const auto navalbase = buildings.GetBuildingInState(vic3_id, vic3::BuildingType::NavalBase);
      if (!navalbase.has_value())
      {
         continue;
      }
      for (const auto& pm: navalbase->GetProductionMethods())
      {
         pm_amounts[pm] += navalbase->GetStaffingLevel();
      }

      auto location = states.states[hoi4_id - 1].GetNavalBaseLocation();
      if (!location)
      {
         continue;
      }

      hoi4::TaskForce task_force{.ships = {}, .location = location.value()};
      for (const auto& tmpl: task_force_templates)
      {
         if (!tmpl.AllVariantsActive(active_variants))
         {
            continue;
         }
         tmpl.AddShipsIfPossible(task_force.ships, ship_names, pm_amounts);
      }
      if (!task_force.ships.empty())
      {
         forces.push_back(task_force);
      }
   }
   return forces;
}


std::vector<hoi4::Unit> ConvertArmies(const std::string& tag,
    const mappers::UnitMapper& unit_mapper,
    const vic3::Buildings& buildings,
    const std::vector<hoi4::DivisionTemplate>& division_templates,
    const hoi4::States& states,
    const std::optional<int>& capital_state)
{
   std::vector<hoi4::Battalion> battalions;
   std::vector<hoi4::Unit> units;
   int default_location = 11666;  // Vienna.
   if (capital_state.has_value())
   {
      auto cap = *capital_state;
      if (cap < states.states.size())
      {
         if (!states.states[cap].GetProvinces().empty())
         {
            default_location = *states.states[cap].GetProvinces().begin();
         }
      }
   }
   for (const auto& [vic3_id, hoi4_id]: states.vic3_state_ids_to_hoi4_state_ids)
   {
      const auto itr = states.hoi4_state_ids_to_owner.find(hoi4_id);
      if (itr == states.hoi4_state_ids_to_owner.end())
      {
         continue;
      }
      if (itr->second != tag)
      {
         continue;
      }
      const auto barracks = buildings.GetBuildingInState(vic3_id, vic3::BuildingType::Barracks);
      if (!barracks.has_value())
      {
         continue;
      }
      auto current = unit_mapper.MakeBattalions(barracks->GetProductionMethods(), barracks->GetStaffingLevel());
      const auto& provs = states.states[hoi4_id - 1].GetProvinces();
      auto pitr = provs.begin();
      for (auto& b: current)
      {
         b.SetLocation(*pitr);
         if (pitr++ == provs.end())
         {
            pitr = provs.begin();
         }
      }
      battalions.insert(battalions.end(), current.begin(), current.end());
   }

   if (battalions.empty())
   {
      return units;
   }

   // Sort by decreasing equipment.
   std::sort(battalions.begin(), battalions.end(), [](const hoi4::Battalion& one, const hoi4::Battalion& two) {
      return one.GetEquipmentScale() > two.GetEquipmentScale();
   });

   while (!battalions.empty())
   {
      std::optional<hoi4::Unit> unit;
      // Try to create division templates in the order they were loaded.
      for (const auto& div_template: division_templates)
      {
         unit = FillTemplate(div_template, battalions, default_location);
         if (!unit.has_value())
         {
            continue;
         }
         units.push_back(*unit);
         break;
      }
      if (!unit.has_value())
      {
         break;
      }
   };

   return units;
}


std::tuple<std::string, std::string, std::string> ConvertLaws(const std::set<std::string>& vic3_laws,
    std::string_view ideology)
{
   // civilian economy with export focus and volunteer military is the default
   std::string economy_law = "civilian_economy";
   std::string trade_law = "export_focus";
   std::string military_law = "volunteer_only";

   // but fascist countries change it up
   if (ideology == "fascism")
   {
      economy_law = "partial_economic_mobilisation";
      trade_law = "limited_exports";
   }

   if (vic3_laws.contains("law_mass_conscription"))
   {
      military_law = "limited_conscription";
   }

   return {economy_law, trade_law, military_law};
}

float ConvertStability(const vic3::World& source_world, const vic3::Country& country)
{
   // TODO once we can calculate legitimacy
   float stability = std::clamp(country.GetLegitimacy(), 0, 100) * 0.8F / 100.0F;
   return stability == 0.0 ? 0.0F : 0.60F;
}


std::string ConvertName(const std::string& vic_name, const commonItems::LocalizationDatabase& vic_localizations)
{
   if (const auto& loc_block = vic_localizations.GetLocalizationBlock(vic_name); loc_block)
   {
      return loc_block->GetLocalization("english");
   }
   Log(LogLevel::Warning) << fmt::format("Missing loc for vic_name: {}.", vic_name);
   return "John";
}

std::set<std::string> ConvertNameSet(const std::set<std::string>& vic_names,
    const commonItems::LocalizationDatabase& vic_localizations)
{
   std::set<std::string> hoi_names;
   for (const auto& vic_name: vic_names)
   {
      hoi_names.emplace(ConvertName(vic_name, vic_localizations));
   }
   return hoi_names;
}

hoi4::NameList ConvertNameList(const std::set<std::string>& primary_cultures,
    const std::map<std::string, vic3::CultureDefinition>& source_cultures,
    const commonItems::LocalizationDatabase& vic_localizations,
    const std::set<std::string>& laws,
    const mappers::LeaderTypeMapper& leader_type_mapper)
{
   hoi4::NameList name_list;
   for (const auto& culture: primary_cultures)
   {
      if (const auto culture_itr = source_cultures.find(culture); culture_itr != source_cultures.end())
      {
         const auto& vic_list = culture_itr->second.GetNameList();

         if (hoi4::HasMonarchs(leader_type_mapper.GetCountryLeaderType(laws), laws))
         {
            name_list.male_names = ConvertNameSet(vic_list.male_regal_first, vic_localizations);
            name_list.female_names = ConvertNameSet(vic_list.female_regal_first, vic_localizations);
            name_list.surnames = ConvertNameSet(vic_list.noble_last, vic_localizations);
         }
         else
         {
            name_list.surnames = ConvertNameSet(vic_list.common_last, vic_localizations);
         }

         // The royal first names can often be empty
         if (name_list.male_names.size() < 5)
         {
            std::ranges::copy(ConvertNameSet(vic_list.male_common_first, vic_localizations),
                std::inserter(name_list.male_names, name_list.male_names.end()));
         }
         if (name_list.female_names.size() < 5)
         {
            std::ranges::copy(ConvertNameSet(vic_list.female_common_first, vic_localizations),
                std::inserter(name_list.female_names, name_list.female_names.end()));
         }
      }
   }
   return name_list;
}


std::map<std::string, float> CalculateRawIdeologySupport(const std::vector<int>& interest_group_ids,
    const std::map<int, vic3::InterestGroup>& interest_groups,
    const vic3::Ideologies& vic3_ideologies,
    const mappers::IdeologyMapper& ideology_mapper,
    bool debug)
{
   std::map<std::string, float> ideology_support;
   std::map<std::string, std::map<std::string, float>> total_ideologies_per_law;

   for (const int interest_group_id: interest_group_ids)
   {
      std::map<std::string, float> ig_ideology_support;
      const auto ig_itr = interest_groups.find(interest_group_id);
      if (ig_itr == interest_groups.end())
      {
         continue;
      }
      const vic3::InterestGroup& interest_group = ig_itr->second;

      for (const auto& [vic3_law, approval_amount]:
          vic3_ideologies.CalculateLawApprovals(interest_group.GetIdeologies()))
      {
         mappers::IdeologyPointsMap ideology_points_map = ideology_mapper.CalculateIdeologyPoints({vic3_law});
         for (auto& [ideology, support]: ideology_points_map)
         {
            float total_support = static_cast<float>(support) * interest_group.GetClout() * approval_amount;
            if (auto [itr, success] = ideology_support.emplace(ideology, total_support); !success)
            {
               itr->second += total_support;
            }
            total_ideologies_per_law[vic3_law][ideology] += total_support;
            ig_ideology_support[ideology] += total_support;
         }
      }
      if (debug)
      {
         Log(LogLevel::Debug) << fmt::format("{}: {:.0f} {:.0f} {:.0f} {:.0f}",
             ig_itr->second.GetType(),
             ig_ideology_support.at("democratic"),
             ig_ideology_support.at("communism"),
             ig_ideology_support.at("fascism"),
             ig_ideology_support.at("neutrality"));
      }
   }

   if (debug)
   {
      for (const auto& law: total_ideologies_per_law)
      {
         Log(LogLevel::Debug) << fmt::format("{} : {:.0f} {:.0f} {:.0f} {:.0f}",
             law.first,
             law.second.at("democratic"),
             law.second.at("communism"),
             law.second.at("fascism"),
             law.second.at("neutrality"));
      }
   }

   if (ideology_support.empty())
   {
      ideology_support.emplace("neutrality", 100.F);
   }

   return ideology_support;
}


std::map<std::string, int> NormalizeIdeologySupport(const std::map<std::string, float>& raw_ideology_support)
{
   // should be impossible to hit, but put in a check in case other functions get modified incorrectly
   if (raw_ideology_support.empty())
   {
      return {};
   }

   const float lowest_support = std::ranges::min_element(raw_ideology_support, [](const auto& a, const auto& b) {
      return a.second < b.second;
   })->second;


   std::map<std::string, float> adjusted_ideology_support;
   for (const auto& [ideology, raw_value]: raw_ideology_support)
   {
      adjusted_ideology_support.emplace(ideology, raw_value - lowest_support);
   }


   const float total_raw_support = std::accumulate(adjusted_ideology_support.begin(),
       adjusted_ideology_support.end(),
       0.F,
       [](float a, const auto& b) {
          return a + b.second;
       });

   std::map<std::string, int> ideology_support;
   if (total_raw_support != 0.F)
   {
      const float normalization_factor = 100.F / total_raw_support;
      for (const auto& [ideology, raw_value]: adjusted_ideology_support)
      {
         int value = static_cast<int>(raw_value * normalization_factor);
         ideology_support.emplace(ideology, value);
      }
   }
   else
   {
      const int normalization_factor = static_cast<int>(100.F / static_cast<float>(adjusted_ideology_support.size()));
      for (const auto& ideology: adjusted_ideology_support | std::views::keys)
      {
         ideology_support.emplace(ideology, normalization_factor);
      }
   }

   ideology_support["neutrality"] = 0;
   const int total_support =
       std::accumulate(ideology_support.begin(), ideology_support.end(), 0, [](int a, const auto& b) {
          return a + b.second;
       });
   ideology_support["neutrality"] = 100 - total_support;

   return ideology_support;
}


std::map<std::string, int> DetermineIdeologySupport(const std::vector<int>& interest_group_ids,
    const std::map<int, vic3::InterestGroup>& interest_groups,
    const vic3::Ideologies& vic3_ideologies,
    const mappers::IdeologyMapper& ideology_mapper,
    bool debug)
{
   const std::map<std::string, float> raw_ideology_support =
       CalculateRawIdeologySupport(interest_group_ids, interest_groups, vic3_ideologies, ideology_mapper, debug);

   return NormalizeIdeologySupport(raw_ideology_support);
}

int DetermineStartingResearchSlots(const vic3::World& source_world, const vic3::Country& source_country)
{
   if (source_country.GetCountryRankCategory(source_world) == vic3::RankCategory::GreatPower)
   {
      return 4;
   }
   else if (source_country.IsRecognized())
   {
      return 3;
   }
   else
   {
      return 2;
   }
}

int ConvertConvoys(const std::string& tag, const hoi4::States& states, const hoi4::ConvoyDistributor& convoys)
{
   int numConvoys = 0;
   for (const auto& [vic3_id, hoi4_id]: states.vic3_state_ids_to_hoi4_state_ids)
   {
      const auto itr = states.hoi4_state_ids_to_owner.find(hoi4_id);
      if (itr == states.hoi4_state_ids_to_owner.end())
      {
         continue;
      }
      if (itr->second != tag)
      {
         continue;
      }
      numConvoys += convoys.ConvoysFromState(vic3_id);
   }
   return numConvoys;
}

}  // namespace



std::optional<hoi4::Country> hoi4::ConvertCountry(const vic3::World& source_world,
    const vic3::Country& source_country,
    const commonItems::LocalizationDatabase& source_localizations,
    const mappers::CountryMapper& country_mapper,
    const States& states,
    const mappers::IdeologyMapper& ideology_mapper,
    const mappers::UnitMapper& unit_mapper,
    const std::vector<mappers::TechMapping>& tech_mappings,
    const std::vector<EquipmentVariant>& all_legacy_ship_variants,
    const std::vector<EquipmentVariant>& all_ship_variants,
    const std::vector<EquipmentVariant>& all_plane_variants,
    const std::vector<EquipmentVariant>& all_tank_variants,
    const std::vector<DivisionTemplate>& division_templates,
    const mappers::CultureGraphicsMapper& culture_graphics_mapper,
    const mappers::LeaderTypeMapper& leader_type_mapper,
    const mappers::CharacterTraitMapper& character_trait_mapper,
    const ConvoyDistributor& convoys,
    const std::vector<hoi4::TaskForceTemplate>& task_force_templates,
    std::map<int, hoi4::Character>& characters,
    std::map<std::string, mappers::CultureQueue>& culture_queues,
    bool debug)
{
   const std::optional<std::string> tag = country_mapper.GetHoiTag(source_country.GetNumber());
   if (!tag.has_value())
   {
      return std::nullopt;
   }

   if (debug)
   {
      Log(LogLevel::Debug) << fmt::format("converting {} (vic {})", tag.value(), source_country.GetTag());
   }

   const std::optional<int> capital_state =
       ConvertCapital(source_country, *tag, states.vic3_state_ids_to_hoi4_state_ids, states.states);
   const std::string ideology = ideology_mapper.GetRulingIdeology(source_country.GetActiveLaws());
   const std::string sub_ideology = ideology_mapper.GetSubIdeology(ideology, source_country.GetActiveLaws());
   const date last_election = ConvertElection(source_country.GetLastElection());
   bool has_elections = source_country.GetLastElection().has_value();
   const hoi4::Technologies technologies =
       ConvertTechnologies(source_country.GetAcquiredTechnologies(source_world), tech_mappings);
   const std::vector<EquipmentVariant>& active_legacy_ship_variants =
       DetermineActiveVariants(all_legacy_ship_variants, technologies);
   const std::vector<EquipmentVariant>& active_ship_variants = DetermineActiveVariants(all_ship_variants, technologies);
   const std::vector<EquipmentVariant>& active_plane_variants =
       DetermineActiveVariants(all_plane_variants, technologies);
   const std::vector<EquipmentVariant>& active_tank_variants = DetermineActiveVariants(all_tank_variants, technologies);
   auto units =
       ConvertArmies(*tag, unit_mapper, source_world.GetBuildings(), division_templates, states, capital_state);
   auto task_forces = ConvertNavies(*tag,
       source_world.GetBuildings(),
       task_force_templates,
       active_ship_variants,
       active_legacy_ship_variants,
       states);

   const auto& [economy_law, trade_law, military_law] = ConvertLaws(source_country.GetActiveLaws(), ideology);

   std::set<std::string> ideas;
   if (source_country.IsDecentralized())
   {
      ideas.insert("decentralized");
   }

   auto numConvoys = ConvertConvoys(*tag, states, convoys);

   const auto [character_ids, spy_ids, monarch_id] = ConvertCharacters(source_world.GetCharacters(),
       *tag,
       ideology,
       sub_ideology,
       source_country,
       source_world.GetInterestGroups(),
       leader_type_mapper,
       character_trait_mapper,
       country_mapper,
       ShouldHaveMonarchIdea(has_elections, source_country.GetActiveLaws()),
       characters,
       culture_queues);
   if (monarch_id.has_value())
   {
      const auto& monarch_itr = characters.find(*monarch_id);
      if (monarch_itr != characters.end())
      {
         ideas.insert(GetMonarchIdeaName(*tag, monarch_itr->second));
      }
   }

   mappers::GraphicsBlock graphics_block =
       culture_graphics_mapper.MatchPrimaryCulturesToGraphics(source_country.GetPrimaryCultures(),
           source_world.GetCultureDefinitions());
   NameList name_list = ConvertNameList(source_country.GetPrimaryCultures(),
       source_world.GetCultureDefinitions(),
       source_localizations,
       source_country.GetActiveLaws(),
       leader_type_mapper);

   std::set<std::string> puppets;
   for (const auto p: source_country.GetPuppets())
   {
      std::optional<std::string> subjectTag = country_mapper.GetHoiTag(p);
      if (!subjectTag.has_value())
      {
         Log(LogLevel::Error) << "Invalid subject relationship between " << source_country.GetNumber()
                              << " and nonexistent country " << p;
         continue;
      }
      puppets.insert(*subjectTag);
   }

   std::optional<std::string> overlord;
   if (const std::optional<int>& source_overlord = source_country.GetOverlord(); source_overlord != std::nullopt)
   {
      overlord = country_mapper.GetHoiTag(*source_overlord);
   }

   const auto ideology_support = DetermineIdeologySupport(source_country.GetInterestGroupIds(),
       source_world.GetInterestGroups(),
       source_world.GetIdeologies(),
       ideology_mapper,
       debug);

   if (debug)
   {
      Log(LogLevel::Debug) << fmt::format("{}: {} {} {} {}",
          tag.value_or(""),
          ideology_support.at("democratic"),
          ideology_support.at("communism"),
          ideology_support.at("fascism"),
          ideology_support.at("neutrality"));
   }

   return Country({.tag = *tag,
       .color = source_country.GetColor(),
       .capital_state = capital_state,
       .ideology = ideology,
       .sub_ideology = sub_ideology,
       .ideology_support = ideology_support,
       .last_election = last_election,
       .has_elections = has_elections,
       .technologies = technologies,
       .legacy_ship_variants = active_legacy_ship_variants,
       .ship_variants = active_ship_variants,
       .plane_variants = active_plane_variants,
       .tank_variants = active_tank_variants,
       .ideas = ideas,
       .economy_law = economy_law,
       .trade_law = trade_law,
       .military_law = military_law,
       .graphics_block = graphics_block,
       .name_list = name_list,
       .character_ids = character_ids,
       .spy_ids = spy_ids,
       .monarch_idea_id = monarch_id,
       .puppets = puppets,
       .overlord = overlord,
       .starting_research_slots = DetermineStartingResearchSlots(source_world, source_country),
       .units = units,
       .stability = ConvertStability(source_world, source_country),
       .convoys = numConvoys,
       .task_forces = task_forces});
}
