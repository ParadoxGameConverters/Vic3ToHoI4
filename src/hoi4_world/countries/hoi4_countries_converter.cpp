#include "src/hoi4_world/countries/hoi4_countries_converter.h"

#include <ranges>

#include "external/fmt/include/fmt/format.h"
#include "src/hoi4_world/military/equipment_variant.h"
#include "src/hoi4_world/military/equipment_variants_importer.h"
#include "src/mappers/culture/culture_graphics_mapper_importer.h"
#include "src/mappers/ideology/ideology_mapper.h"
#include "src/mappers/ideology/ideology_mapper_importer.h"



namespace
{

void LogIdeologies(const std::map<std::string, hoi4::Country>& countries)
{
   std::map<std::string, std::map<std::string, int>> ideology_counts;
   for (const hoi4::Country& country: countries | std::views::values)
   {
      const std::string& ideology = country.GetIdeology();
      const std::string& sub_ideology = country.GetSubIdeology();

      if (const auto [itr, success] = ideology_counts.emplace(ideology, std::map<std::string, int>{{sub_ideology, 1}});
          !success)
      {
         if (const auto [sub_itr, sub_success] = itr->second.emplace(sub_ideology, 1); !sub_success)
         {
            sub_itr->second++;
         }
      }
   }

   Log(LogLevel::Info) << "\tIdeology data:";
   for (const auto& [ideology, sub_data]: ideology_counts)
   {
      for (const auto& [sub_ideology, count]: sub_data)
      {
         Log(LogLevel::Info) << fmt::format("\t\t{}, {} - {} countries", ideology, sub_ideology, count);
      }
   }
}

}  // namespace



std::map<std::string, hoi4::Country> hoi4::ConvertCountries(const std::map<int, vic3::Country>& source_countries,
    const std::map<int, std::set<std::string>>& source_technologies,
    const std::map<std::string, vic3::CultureDefinition>& source_cultures,
    const commonItems::LocalizationDatabase& source_localizations,
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

   const mappers::CultureGraphicsMapper culture_graphics_mapper =
       mappers::ImportCultureGraphicsMapper("configurables/culture_graphics.txt");

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
          source_cultures,
          source_localizations,
          country_mapper,
          vic3_state_ids_to_hoi4_state_ids,
          states,
          ideology_mapper,
          tech_mappings,
          all_legacy_ship_variants,
          all_ship_variants,
          all_plane_variants,
          all_tank_variants,
          culture_graphics_mapper);
      if (new_country.has_value())
      {
         countries.emplace(new_country->GetTag(), *new_country);
      }
   }

   LogIdeologies(countries);

   return countries;
}