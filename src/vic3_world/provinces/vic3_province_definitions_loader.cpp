#define STB_IMAGE_IMPLEMENTATION

#include "src/vic3_world/provinces/vic3_province_definitions_loader.h"

#include <external/fmt/include/fmt/format.h>
#include <external/stb/stb_image.h>

#include <filesystem>
#include <stdexcept>
#include <unordered_set>

#include "external/commonItems/CommonRegexes.h"
#include "external/commonItems/ParserHelpers.h"



namespace
{

std::map<std::string, int> DetermineProvinceOrdering(const commonItems::ModFilesystem& filesystem)
{
   int width = 0;
   int height = 0;
   int depth = 0;
   unsigned char* data =
       stbi_load(filesystem.GetActualFileLocation("map_data/provinces.png")->c_str(), &width, &height, &depth, 0);
   if (data == nullptr)
   {
      throw std::runtime_error("Could not load map_data/provinces.png");
   }

   std::map<std::string, int> province_indexes;
   std::unordered_set<std::string> found_provinces;
   for (int y = height - 1; y >= 0; --y)
   {
      for (int x = 0; x < width; ++x)
      {
         const int pixel = y * width + x;
         const uint8_t r = data[pixel * depth + 0];
         const uint8_t g = data[pixel * depth + 1];
         const uint8_t b = data[pixel * depth + 2];
         const std::string province = fmt::format("x{:0>2X}{:0>2X}{:0>2X}", r, g, b);
         if (auto [_, success] = found_provinces.insert(province); success)
         {
            // successful insertion means this is the first we saw this province's color
            province_indexes.emplace(province, static_cast<int>(province_indexes.size()));
         }
      }
   }
   stbi_image_free(data);

   return province_indexes;
}

}  // namespace



vic3::ProvinceDefinitions vic3::LoadProvinceDefinitions(const StateRegions& state_regions,
    const commonItems::ModFilesystem& filesystem)
{
   std::vector<std::string> province_definitions;

   const std::map<std::string, int> province_indexes = DetermineProvinceOrdering(filesystem);

   commonItems::parser region_parser;
   region_parser.registerKeyword("states",
       [state_regions, province_indexes, &province_definitions](std::istream& input_stream) {
          std::vector<std::string> states = commonItems::stringList{input_stream}.getStrings();

          const auto& region_indexes = state_regions.region_indexes;
          std::ranges::sort(states, [region_indexes](const std::string& a, const std::string& b) {
             return region_indexes.at(a) < region_indexes.at(b);
          });

          for (const std::string& state: states)
          {
             const auto region = state_regions.name_to_region_map.find(state);
             std::vector<ProvinceId> provinces(region->second.GetProvinces().begin(),
                 region->second.GetProvinces().end());
             std::ranges::sort(provinces, [province_indexes](const std::string& a, const std::string& b) {
                return province_indexes.at(a) < province_indexes.at(b);
             });
             for (const ProvinceId& province_id: provinces)
             {
                province_definitions.push_back(province_id);
             }
          }
       });
   region_parser.registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);

   commonItems::parser file_parser;
   file_parser.registerRegex(commonItems::catchallRegex,
       [&region_parser]([[maybe_unused]] const std::string& unused, std::istream& input_stream) {
          region_parser.parseStream(input_stream);
       });
   file_parser.IgnoreAndLogUnregisteredItems();

   for (const std::string& strategic_region: filesystem.GetAllFilesInFolder("common/strategic_regions"))
   {
      if (std::filesystem::path(strategic_region).extension() != ".txt")
      {
         continue;
      }
      file_parser.parseFile(strategic_region);
   }

   return ProvinceDefinitions(province_definitions);
}