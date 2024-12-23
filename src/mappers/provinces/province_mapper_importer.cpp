#include "src/mappers/provinces/province_mapper_importer.h"

#include <external/commonItems/CommonRegexes.h>
#include <external/commonItems/Log.h>
#include <external/commonItems/ModLoader/ModFilesystem.h>
#include <external/commonItems/ParserHelpers.h>
#include <external/fmt/include/fmt/format.h>

#include <fstream>

#include "src/vic3_world/states/state_regions_importer.h"
#include "src/vic3_world/world/vic3_world.h"



namespace
{

std::optional<int> GetNextProvinceNumFromFile(std::ifstream& definitions)
{
   std::string line;
   getline(definitions, line);

   if (const auto pos = line.find_first_of(';'); pos != std::string::npos)
   {
      try
      {
         return stoi(line.substr(0, pos));
      }
      catch (...)
      {
         Log(LogLevel::Warning) << "Bad line in /map/definition.csv: " << line;
      }

      return std::nullopt;
   }

   return std::nullopt;
}



void VerifyProvinceIsMapped(const mappers::Hoi4ToVic3ProvinceMapping& hoi4_to_vic3_province_map, int province_number)
{
   if (!hoi4_to_vic3_province_map.contains(province_number))
   {
      Log(LogLevel::Warning) << "No mapping for Hoi4 province " << province_number;
   }
}

std::map<std::string, std::string> GenerateProvinceToStateMap(
    const std::map<std::string, vic3::StateRegion> vic3_state_regions)
{
   std::map<std::string, std::string> province_to_state_map;
   for (const auto& state: vic3_state_regions)
   {
      for (const auto& province: state.second.GetProvinces())
      {
         province_to_state_map[province] = state.first;
      }
   }
   return province_to_state_map;
}

void AreVic3ProvincesFromSameState(const std::vector<std::string>& provinces_from_map,
    const std::map<std::string, std::string>& province_to_state_map)
{
   std::set<std::string> state_names;
   for (const auto& province: provinces_from_map)
   {
      if (auto linked_state = province_to_state_map.find(province); linked_state != province_to_state_map.end())
      {
         state_names.insert(linked_state->second);
      }
   }
   // if the provinces are part of multiple different state
   if (state_names.size() > 1)
   {
      for (const auto& province_from_map: provinces_from_map)
      {
         Log(LogLevel::Debug) << fmt::format(
             "Province {} is designated as part of {} in Vic3 data and is placed in a mapping with provinces from "
             "other states.",
             province_from_map,
             province_to_state_map.at(province_from_map));
      }
   }
}


void IsMappingInWrongRegion(std::string_view current_region,
    const std::vector<std::string>& provinces_from_map,
    const std::map<std::string, std::string>& province_to_state_map)
{
   for (const auto& province: provinces_from_map)
   {
      auto linked_state = province_to_state_map.find(province);
      if (linked_state != province_to_state_map.end() && linked_state->second != current_region)
      {
         Log(LogLevel::Debug) << fmt::format(
             "Province {} is designated as part of {} in Vic3 data and is placed in a mapping in the {} region.",
             province,
             province_to_state_map.at(province),
             current_region);
      }
   }
}


void CheckAllHoi4ProvincesMapped(const mappers::Hoi4ToVic3ProvinceMapping& hoi4_to_vic3_province_map,
    const commonItems::ModFilesystem& filesystem)
{
   const auto definition_location = filesystem.GetActualFileLocation("/map/definition.csv");
   if (!definition_location.has_value())
   {
      throw std::runtime_error("Could not find /map/definition.csv");
   }

   std::ifstream definitions(*definition_location);
   if (!definitions.is_open())
   {
      throw std::runtime_error(fmt::format("Could not open {}", *definition_location));
   }

   while (true)
   {
      auto province_number = GetNextProvinceNumFromFile(definitions);
      if (!province_number)
      {
         break;
      }
      if (*province_number == 0)
      {
         continue;
      }

      VerifyProvinceIsMapped(hoi4_to_vic3_province_map, *province_number);
   }

   definitions.close();
}

}  // namespace

mappers::ProvinceMapperImporter::ProvinceMapperImporter(const commonItems::ModFilesystem& filesystem,
    const std::map<std::string, vic3::StateRegion>& vic3_state_regions)
{
   province_to_state_map_ = GenerateProvinceToStateMap(vic3_state_regions);

   version_parser_.registerRegex(R"(\d\.[\d]+\.\d)",
       [this, &filesystem](const std::string& unused, std::istream& input_stream) {
          mapping_parser_.parseStream(input_stream);
          CheckAllHoi4ProvincesMapped(hoi4_to_vic3_province_map_, filesystem);
       });

   mapping_parser_.registerKeyword("link", [this, vic3_state_regions](std::istream& input_stream) {
      const auto the_mapping = mapping_importer_.ImportProvinceMapping(input_stream);
      if (the_mapping.comment.has_value())
      {
         current_region_ = the_mapping.comment.value();
      }
      if (the_mapping.vic3_provinces.empty() && the_mapping.hoi4_provinces.empty())
      {
         return;
      }

      if (!vic3_state_regions.empty())
      {
         AreVic3ProvincesFromSameState(the_mapping.vic3_provinces, province_to_state_map_);
         IsMappingInWrongRegion(current_region_, the_mapping.vic3_provinces, province_to_state_map_);
      }

      for (auto color: the_mapping.vic3_provinces)
      {
         const auto [unused, success] = vic3_to_hoi4_province_map_.emplace(color, the_mapping.hoi4_provinces);
         if (!success)
         {
            Log(LogLevel::Warning) << fmt::format("Vic3 province {} was in multiple mappings.", color);
         }
      }
      for (auto num: the_mapping.hoi4_provinces)
      {
         const auto [unused, success] = hoi4_to_vic3_province_map_.emplace(num, the_mapping.vic3_provinces);
         if (!success)
         {
            Log(LogLevel::Warning) << fmt::format("Hoi4 province {} was in multiple mappings.", num);
         }
      }
   });
   mapping_parser_.IgnoreAndLogUnregisteredItems();
}


mappers::ProvinceMapper mappers::ProvinceMapperImporter::ImportProvinceMappings()
{
   Log(LogLevel::Info) << "Importing province mappings.";
   vic3_to_hoi4_province_map_.clear();
   hoi4_to_vic3_province_map_.clear();

   version_parser_.parseFile("configurables/province_mappings.txt");

   return ProvinceMapper(vic3_to_hoi4_province_map_, hoi4_to_vic3_province_map_);
}