#include "src/hoi4_world/countries/hoi4_countries_converter.h"

#include <ranges>

#include "src/hoi4_world/military/equipment_variant.h"
#include "src/hoi4_world/military/equipment_variants_importer.h"
#include "src/mappers/ideology/ideology_mapper.h"
#include "src/mappers/ideology/ideology_mapper_importer.h"



std::map<std::string, hoi4::Country> hoi4::ConvertCountries(const std::map<int, vic3::Country>& source_countries,
    const std::map<int, std::set<std::string>>& source_technologies,
    const mappers::CountryMapper& country_mapper,
    const std::map<int, int>& vic3_state_ids_to_hoi4_state_ids,
    const std::vector<State>& states,
    const std::vector<mappers::TechMapping>& tech_mappings)
{
   std::map<std::string, Country> countries;

   const mappers::IdeologyMapper ideology_mapper = mappers::ImportIdeologyMapper("configurables/ideology_mappings.txt");

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