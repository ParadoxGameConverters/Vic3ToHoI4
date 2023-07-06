#include "src/hoi4_world/countries/hoi4_country_converter.h"

#include <numeric>

#include "external/fmt/include/fmt/format.h"
#include "src/hoi4_world/characters/hoi4_character_converter.h"
#include "src/hoi4_world/characters/hoi4_characters_converter.h"
#include "src/hoi4_world/technology/technologies_converter.h"
#include "src/mappers/character/leader_type_mapper.h"


namespace
{

int FloorMod(const int lhs, const int rhs)
{
   return (lhs % rhs + rhs) % rhs;
}


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

}  // namespace



std::optional<hoi4::Country> hoi4::ConvertCountry(const vic3::Country& source_country,
    const std::set<std::string>& source_technologies,
    const std::map<std::string, vic3::CultureDefinition>& source_cultures,
    const commonItems::LocalizationDatabase& source_localizations,
    const mappers::CountryMapper& country_mapper,
    const std::map<int, int>& vic3_state_ids_to_hoi4_state_ids,
    const std::vector<State>& states,
    const mappers::IdeologyMapper& ideology_mapper,
    const std::vector<mappers::TechMapping>& tech_mappings,
    const std::vector<EquipmentVariant>& all_legacy_ship_variants,
    const std::vector<EquipmentVariant>& all_ship_variants,
    const std::vector<EquipmentVariant>& all_plane_variants,
    const std::vector<EquipmentVariant>& all_tank_variants,
    const mappers::CultureGraphicsMapper& culture_graphics_mapper,
    const std::map<int, vic3::Character>& vic3_characters,
    const mappers::LeaderTypeMapper& leader_type_mapper,
    const std::map<int, vic3::InterestGroup>& igs,
    std::map<int, hoi4::Character>& characters,
    std::map<std::string, mappers::CultureQueue>& culture_queues)
{
   const std::optional<std::string> tag = country_mapper.GetHoiTag(source_country.GetNumber());
   if (!tag.has_value())
   {
      return std::nullopt;
   }

   const std::optional<int> capital_state =
       ConvertCapital(source_country, *tag, vic3_state_ids_to_hoi4_state_ids, states);
   const std::string ideology = ideology_mapper.GetRulingIdeology(source_country.GetActiveLaws());
   const std::string sub_ideology = ideology_mapper.GetSubIdeology(ideology, source_country.GetActiveLaws());
   const date last_election = ConvertElection(source_country.GetLastElection());
   const Technologies technologies = ConvertTechnologies(source_technologies, tech_mappings);
   const std::vector<EquipmentVariant>& active_legacy_ship_variants =
       DetermineActiveVariants(all_legacy_ship_variants, technologies);
   const std::vector<EquipmentVariant>& active_ship_variants = DetermineActiveVariants(all_ship_variants, technologies);
   const std::vector<EquipmentVariant>& active_plane_variants =
       DetermineActiveVariants(all_plane_variants, technologies);
   const std::vector<EquipmentVariant>& active_tank_variants = DetermineActiveVariants(all_tank_variants, technologies);

   std::set<std::string> ideas;
   if (source_country.IsDecentralized())
   {
      ideas.insert("decentralized");
   }

   const auto [leader_ids, spy_ids] = ConvertCharacters(characters,
       vic3_characters,
       *tag,
       sub_ideology,
       source_country,
       igs,
       leader_type_mapper,
       culture_queues,
       country_mapper);
   mappers::GraphicsBlock graphics_block =
       culture_graphics_mapper.MatchPrimaryCulturesToGraphics(source_country.GetPrimaryCultures(), source_cultures);
   NameList name_list = ConvertNameList(source_country.GetPrimaryCultures(),
       source_cultures,
       source_localizations,
       source_country.GetActiveLaws(),
       leader_type_mapper);

   return Country({.tag = *tag,
       .color = source_country.GetColor(),
       .capital_state = capital_state,
       .ideology = ideology,
       .sub_ideology = sub_ideology,
       .last_election = last_election,
       .has_elections = source_country.GetLastElection().has_value(),
       .technologies = technologies,
       .legacy_ship_variants = active_legacy_ship_variants,
       .ship_variants = active_ship_variants,
       .plane_variants = active_plane_variants,
       .tank_variants = active_tank_variants,
       .ideas = ideas,
       .graphics_block = graphics_block,
       .name_list = name_list,
       .leader_ids = leader_ids,
       .spy_ids = spy_ids});
}