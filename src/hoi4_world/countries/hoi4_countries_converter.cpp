#include "src/hoi4_world/countries/hoi4_countries_converter.h"

#include <external/fmt/include/fmt/format.h>

#include <ranges>

#include "src/hoi4_world/military/convoy_distributor.h"
#include "src/hoi4_world/military/convoy_distributor_builder.h"
#include "src/hoi4_world/military/division_templates_importer.h"
#include "src/hoi4_world/military/equipment_variant.h"
#include "src/hoi4_world/military/equipment_variants_importer.h"
#include "src/hoi4_world/military/task_force_template.h"
#include "src/hoi4_world/military/task_force_templates_builder.h"
#include "src/mappers/character/character_trait_mapper_importer.h"
#include "src/mappers/character/leader_type_mapper_importer.h"
#include "src/mappers/culture/culture_graphics_mapper_importer.h"
#include "src/mappers/ideology/ideology_mapper.h"
#include "src/mappers/ideology/ideology_mapper_importer.h"
#include "src/mappers/unit/unit_mapper.h"
#include "src/mappers/unit/unit_mapper_importer.h"
#include "src/mappers/world/world_mapper.h"



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


namespace hoi4
{

std::map<std::string, Country> ConvertCountries(const vic3::World source_world,
    const mappers::WorldMapper& world_mapper,
    const commonItems::LocalizationDatabase& source_localizations,
    const States& states,
    std::map<int, Character>& characters,
    std::map<std::string, mappers::CultureQueue>& culture_queues,
    bool debug)
{
   std::map<std::string, Country> countries;

   const mappers::IdeologyMapper ideology_mapper = mappers::ImportIdeologyMapper("configurables/ideology_mappings.txt");
   const mappers::UnitMapper unit_mapper = mappers::ImportUnitMapper("configurables/unit_mappings.txt");

   const std::vector<EquipmentVariant> all_legacy_ship_variants =
       ImportEquipmentVariants("configurables/legacy_ship_types.txt");
   const std::vector<EquipmentVariant> all_ship_variants = ImportEquipmentVariants("configurables/ship_types.txt");
   const std::vector<EquipmentVariant> all_plane_variants = ImportEquipmentVariants("configurables/plane_designs.txt");
   const std::vector<EquipmentVariant> all_tank_variants = ImportEquipmentVariants("configurables/tank_designs.txt");
   const std::vector<DivisionTemplate> division_templates =
       ImportDivisionTemplates("configurables/division_templates.txt");
   const std::vector<TaskForceTemplate> task_force_templates =
       ImportTaskForceTemplates("configurables/task_force_templates.txt");

   const mappers::LeaderTypeMapper leader_type_mapper =
       mappers::ImportLeaderTypeMapper("configurables/leader_type_mappings.txt");
   const mappers::CharacterTraitMapper character_trait_mapper =
       mappers::ImportCharacterTraitMapper("configurables/character_traits.txt");
   ConvoyDistributor convoys = BuildConvoyDistributor("configurables/convoy_config.txt");
   convoys.CalculateStateWeights(source_world);

   for (const auto& source_country: source_world.GetCountries() | std::views::values)
   {
      if (source_country.IsDead())
      {
         continue;
      }

      std::optional<Country> new_country = ConvertCountry(source_world,
          source_country,
          source_localizations,
          world_mapper.country_mapper,
          states,
          ideology_mapper,
          unit_mapper,
          world_mapper.tech_mapper,
          all_legacy_ship_variants,
          all_ship_variants,
          all_plane_variants,
          all_tank_variants,
          division_templates,
          world_mapper.culture_graphics_mapper,
          leader_type_mapper,
          character_trait_mapper,
          convoys,
          task_force_templates,
          characters,
          culture_queues,
          debug);
      if (new_country.has_value())
      {
         countries.emplace(new_country->GetTag(), *new_country);
      }
   }

   for (Country& country: countries | std::views::values)
   {
      std::set<std::string> puppets_to_remove;
      for (const std::string& puppet_tag: country.GetPuppets())
      {
         if (const auto puppet = countries.find(puppet_tag); puppet != countries.end())
         {
            // lacking a capital state indicates there was no owned territory
            if (!puppet->second.GetCapitalState().has_value())
            {
               puppets_to_remove.insert(puppet_tag);
            }
         }
         else
         {
            puppets_to_remove.insert(puppet_tag);
         }
      }

      country.RemovePuppets(puppets_to_remove);
   }

   LogIdeologies(countries);

   return countries;
}

}  // namespace hoi4
