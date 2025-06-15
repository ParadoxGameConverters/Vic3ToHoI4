#include "src/vic3_world/states/state_regions_importer.h"

#include <external/commonItems/CommonRegexes.h>
#include <external/commonItems/Log.h>
#include <external/commonItems/Parser.h>
#include <external/commonItems/ParserHelpers.h>
#include <external/fmt/include/fmt/format.h>

#include <cctype>
#include <filesystem>



using std::filesystem::path;



vic3::StateRegions vic3::ImportStateRegions(const commonItems::ModFilesystem& filesystem)
{
   std::map<std::string, vic3::StateRegion> name_to_region_map;
   std::map<std::string, int> region_indexes;

   std::map<ProvinceId, ProvinceType> significant_provinces;
   std::set<ProvinceId> provinces;

   commonItems::parser region_parser;
   region_parser.registerKeyword("provinces", [&provinces](std::istream& input_stream) {
      for (const auto& province: commonItems::getStrings(input_stream))
      {
         ProvinceId transformed_province = province;
#pragma warning(push)
#pragma warning(disable : 4242)
         std::transform(++transformed_province.begin(),
             transformed_province.end(),
             ++transformed_province.begin(),
             ::toupper);
#pragma warning(pop)
         transformed_province[0] = 'x';
         provinces.insert(transformed_province);
      }
   });
   region_parser.registerRegex("city|port|farm|mine|wood",
       [&significant_provinces](const std::string& significant_province_type, std::istream& input_stream) {
          ProvinceId province = commonItems::getString(input_stream);
          if (province.empty())
          {
             throw std::runtime_error("State regions have been corrupted. Verify your Vic3 install and mods.");
          }
#pragma warning(push)
#pragma warning(disable : 4242)
          std::transform(++province.begin(), province.end(), ++province.begin(), ::toupper);
#pragma warning(pop)
          province[0] = 'x';
          if (auto [itr, success] = significant_provinces.emplace(province, significant_province_type); !success)
          {
             if ((significant_province_type == "city") ||
                 (significant_province_type == "port" && itr->second != "city"))
             {
                itr->second = significant_province_type;
             }
          }
       });
   region_parser.IgnoreUnregisteredItems();

   commonItems::parser file_parser;
   file_parser.registerRegex(commonItems::catchallRegex,
       [&name_to_region_map, &region_indexes, &region_parser, &significant_provinces, &provinces](
           const std::string& region_name,
           std::istream& input_stream) {
          significant_provinces.clear();
          provinces.clear();
          region_parser.parseStream(input_stream);
          StateRegion new_region(significant_provinces, provinces);
          if (auto [itr, success] = name_to_region_map.emplace(region_name, new_region); !success)
          {
             throw std::runtime_error(fmt::format(
                 "Region {} was repeatedly defined. This indicates irrecoverable mod corruption. If this was an EU4 to "
                 "Vic3 conversion mod, delete your copy of EU4 to Vic3 and download a fresh copy, delete your mod and "
                 "reconvert, do a test conversion to verify this error goes away, then play through Vic3 again.",
                 region_name));
          }
          region_indexes.emplace(region_name, static_cast<int>(region_indexes.size()));
       });

   for (const path& state_regions_file: filesystem.GetAllFilesInFolder("map_data/state_regions"))
   {
      file_parser.parseFile(state_regions_file);
   }

   return {.name_to_region_map = name_to_region_map, .region_indexes = region_indexes};
}