#include "src/hoi4_world/countries/hoi4_country_converter.h"

#include <numeric>

#include "src/hoi4_world/technology/technologies_converter.h"



namespace
{


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
   // TODO: Fast-forward election to proper time before startdate.
   return vic_election.value_or("1936.1.1");
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

}  // namespace



std::optional<hoi4::Country> hoi4::ConvertCountry(const vic3::Country& source_country,
    const std::set<std::string>& source_technologies,
    const mappers::CountryMapper& country_mapper,
    const std::map<int, int>& vic3_state_ids_to_hoi4_state_ids,
    const std::vector<State>& states,
    const mappers::IdeologyMapper& ideology_mapper,
    const std::vector<mappers::TechMapping>& tech_mappings,
    const std::vector<EquipmentVariant>& all_legacy_ship_variants,
    const std::vector<EquipmentVariant>& all_ship_variants,
    const std::vector<EquipmentVariant>& all_plane_variants,
    const std::vector<EquipmentVariant>& all_tank_variants)
{
   const std::optional<std::string> tag = country_mapper.GetHoiTag(source_country.GetNumber());
   if (!tag.has_value())
   {
      return std::nullopt;
   }

   const std::optional<int> capital_state =
       ConvertCapital(source_country, *tag, vic3_state_ids_to_hoi4_state_ids, states);
   const std::string ideology = ideology_mapper.GetRulingIdeology(source_country.GetActiveLaws());
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

   return Country({.tag = *tag,
       .color = source_country.GetColor(),
       .capital_state = capital_state,
       .ideology = ideology,
       .last_election = last_election,
       .technologies = technologies,
       .legacy_ship_variants = active_legacy_ship_variants,
       .ship_variants = active_ship_variants,
       .plane_variants = active_plane_variants,
       .tank_variants = active_tank_variants,
       .ideas = ideas});
}