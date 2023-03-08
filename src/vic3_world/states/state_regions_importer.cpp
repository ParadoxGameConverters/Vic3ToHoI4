#include "src/vic3_world/states/state_regions_importer.h"

#include "external/commonItems/CommonRegexes.h"
#include "external/commonItems/Parser.h"
#include "external/commonItems/ParserHelpers.h"



std::map<std::string, vic3::StateRegion> vic3::ImportStateRegions(const commonItems::ModFilesystem filesystem)
{
   std::map<std::string, vic3::StateRegion> state_regions;

   std::map<std::string, std::string> significant_provinces;

   commonItems::parser region_parser;
   region_parser.registerRegex("city|port|farm|mine|wood",
       [&significant_provinces](const std::string& significant_province_type, std::istream& input_stream) {
          std::string province = commonItems::getString(input_stream);
          std::transform(++province.begin(), province.end(), ++province.begin(), ::toupper);
          province[0] = 'x';
          significant_provinces.emplace(province, significant_province_type);
       });
   region_parser.IgnoreUnregisteredItems();

   commonItems::parser file_parser;
   file_parser.registerRegex(commonItems::catchallRegex,
       [&state_regions, &region_parser, &significant_provinces](const std::string& region_name,
           std::istream& input_stream) {
          significant_provinces.clear();
          region_parser.parseStream(input_stream);
          state_regions.emplace(region_name, StateRegion(significant_provinces));
       });

   for (const auto& state_regions_file: filesystem.GetAllFilesInFolder("game/map_data/state_regions"))
   {
      file_parser.parseFile(state_regions_file);
   }

   return state_regions;
}