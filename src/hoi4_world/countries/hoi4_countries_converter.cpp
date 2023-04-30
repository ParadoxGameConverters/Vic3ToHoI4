#include "src/hoi4_world/countries/hoi4_countries_converter.h"

#include <ranges>

#include "src/hoi4_world/military/equipment_variant.h"
#include "src/hoi4_world/military/equipment_variants_importer.h"
#include "src/mappers/ideology/ideology_mapper.h"



std::map<std::string, hoi4::Country> hoi4::ConvertCountries(const std::map<int, vic3::Country>& source_countries,
    const std::map<int, std::set<std::string>>& source_technologies,
    const mappers::CountryMapper& country_mapper,
    const std::map<int, int>& vic3_state_ids_to_hoi4_state_ids,
    const std::vector<State>& states,
    const std::vector<mappers::TechMapping>& tech_mappings)
{
   std::map<std::string, Country> countries;

   const mappers::IdeologyMapper ideology_mapper({
       // governance principles
       {"law_chiefdom",
           {
               {"democratic", -25},
               {"communism", -50},
               {"fascism", -25},
               {"neutrality", 25},
           }},
       {"law_monarchy",
           {
               {"democratic", 25},
               {"communism", -100},
               {"fascism", 0},
               {"neutrality", 50},
           }},
       {"law_presidential_republic",
           {
               {"democratic", 50},
               {"communism", 0},
               {"fascism", 50},
               {"neutrality", 25},
           }},
       {"law_parliamentary_republic",
           {
               {"democratic", 50},
               {"communism", 50},
               {"fascism", 25},
               {"neutrality", 25},
           }},
       {"law_theocracy",
           {
               {"democratic", 25},
               {"communism", -50},
               {"fascism", 25},
               {"neutrality", 50},
           }},
       {"law_council_republic",
           {
               {"democratic", 25},
               {"communism", 100},
               {"fascism", -50},
               {"neutrality", -25},
           }},
       // distribution of power
       {"law_autocracy",
           {
               {"democratic", -100},
               {"communism", 100},
               {"fascism", 100},
               {"neutrality", 100},
           }},
       {"law_oligarchy",
           {
               {"democratic", 25},
               {"communism", 50},
               {"fascism", 50},
               {"neutrality", 100},
           }},
       {"law_elder_council",
           {
               {"democratic", -50},
               {"communism", -50},
               {"fascism", 0},
               {"neutrality", 50},
           }},
       {"law_landed_voting",
           {
               {"democratic", 25},
               {"communism", -50},
               {"fascism", 0},
               {"neutrality", 50},
           }},
       {"law_wealth_voting",
           {
               {"democratic", 25},
               {"communism", -50},
               {"fascism", 0},
               {"neutrality", 50},
           }},
       {"law_census_voting",
           {
               {"democratic", 50},
               {"communism", -50},
               {"fascism", 25},
               {"neutrality", 50},
           }},
       {"law_universal_suffrage",
           {
               {"democratic", 100},
               {"communism", 100},
               {"fascism", -25},
               {"neutrality", 25},
           }},
       {"law_anarchy",
           {
               {"democratic", -10'000},
               {"communism", 10'000},
               {"fascism", -10'000},
               {"neutrality", -10'000},
           }},
       // citizenship
       {"law_ethnostate",
           {
               {"democratic", -50},
               {"communism", -100},
               {"fascism", 100},
               {"neutrality", 25},
           }},
       {"law_national_supremacy",
           {
               {"democratic", -25},
               {"communism", 0},
               {"fascism", 75},
               {"neutrality", 50},
           }},
       {"law_racial_segregation",
           {
               {"democratic", 25},
               {"communism", -25},
               {"fascism", 50},
               {"neutrality", 50},
           }},
       {"law_cultural_exclusion",
           {
               {"democratic", 50},
               {"communism", 0},
               {"fascism", 25},
               {"neutrality", 50},
           }},
       {"law_multicultural",
           {
               {"democratic", 75},
               {"communism", 75},
               {"fascism", -50},
               {"neutrality", 0},
           }},
       // church and state
       {"law_state_religion",
           {
               {"democratic", 0},
               {"communism", -75},
               {"fascism", 50},
               {"neutrality", 75},
           }},
       {"law_freedom_of_conscience",
           {
               {"democratic", 25},
               {"communism", -50},
               {"fascism", 0},
               {"neutrality", 50},
           }},
       {"law_total_separation",
           {
               {"democratic", 50},
               {"communism", 50},
               {"fascism", 0},
               {"neutrality", 0},
           }},
       // bureaucracy
       {"law_hereditary_bureaucrats",
           {
               {"democratic", -25},
               {"communism", -75},
               {"fascism", -25},
               {"neutrality", 50},
           }},
       {"law_appointed_bureaucrats",
           {
               {"democratic", 25},
               {"communism", 50},
               {"fascism", 50},
               {"neutrality", 25},
           }},
       {"law_elected_bureaucrats",
           {
               {"democratic", 50},
               {"communism", 25},
               {"fascism", 0},
               {"neutrality", 0},
           }},
   });

   const std::vector<EquipmentVariant> all_legacy_ship_variants =
       ImportEquipmentVariants("configurables/legacy_ship_types.txt");
   const std::vector<EquipmentVariant> all_ship_variants = ImportEquipmentVariants("configurables/ship_types.txt");
   const std::vector<EquipmentVariant> all_plane_variants = ImportEquipmentVariants("configurables/plane_designs.txt");
   const std::vector<EquipmentVariant> all_tank_variants = ImportEquipmentVariants("configurables/tank_designs.txt");

   for (const auto& [country_number, source_country]: source_countries)
   {
      std::set<std::string> source_country_technologies;
      if (const auto& source_technologies_itr = source_technologies.find(country_number);
          source_technologies_itr != source_technologies.end())
      {
         source_country_technologies = source_technologies_itr->second;
      }

      std::optional<Country> new_country = ConvertCountry(source_country,
          source_country_technologies,
          country_mapper,
          vic3_state_ids_to_hoi4_state_ids,
          states,
          ideology_mapper,
          tech_mappings,
          all_legacy_ship_variants,
          all_ship_variants,
          all_plane_variants,
          all_tank_variants);
      if (new_country.has_value())
      {
         countries.emplace(new_country->GetTag(), *new_country);
      }
   }

   return countries;
}