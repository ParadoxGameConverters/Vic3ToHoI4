#include "src/mappers/provinces/province_mapper_importer.h"

#include <fstream>

#include "external/commonItems/CommonRegexes.h"
#include "external/commonItems/Log.h"
#include "external/commonItems/ModLoader/ModFilesystem.h"
#include "external/commonItems/ParserHelpers.h"
#include "external/fmt/include/fmt/format.h"



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



mappers::ProvinceMapperImporter::ProvinceMapperImporter(const commonItems::ModFilesystem& filesystem)
{
   version_parser_.registerRegex(R"(\d\.[\d]+\.\d)",
       [this, &filesystem](const std::string& unused, std::istream& input_stream) {
          mapping_parser_.parseStream(input_stream);
          CheckAllHoi4ProvincesMapped(hoi4_to_vic3_province_map_, filesystem);
       });

   mapping_parser_.registerKeyword("link", [this](std::istream& input_stream) {
      const auto the_mapping = mapping_importer_.ImportProvinceMapping(input_stream);
      if (the_mapping.vic3_provinces.empty() && the_mapping.hoi4_provinces.empty())
      {
         return;
      }

      for (auto color: the_mapping.vic3_provinces)
      {
         vic3_to_hoi4_province_map_.emplace(color, the_mapping.hoi4_provinces);
      }
      for (auto num: the_mapping.hoi4_provinces)
      {
         hoi4_to_vic3_province_map_.emplace(num, the_mapping.vic3_provinces);
      }
   });
   mapping_parser_.IgnoreAndLogUnregisteredItems();
}


mappers::ProvinceMapper mappers::ProvinceMapperImporter::ImportProvinceMappings()
{
   vic3_to_hoi4_province_map_.clear();
   hoi4_to_vic3_province_map_.clear();

   version_parser_.parseFile("configurables/province_mappings.txt");

   return ProvinceMapper(vic3_to_hoi4_province_map_, hoi4_to_vic3_province_map_);
}