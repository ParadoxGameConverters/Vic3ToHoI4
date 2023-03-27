#include "src/hoi4_world/military/equipment_variants_importer.h"

#include "external/commonItems/CommonRegexes.h"
#include "external/commonItems/Parser.h"
#include "external/commonItems/ParserHelpers.h"



std::vector<hoi4::EquipmentVariant> hoi4::ImportEquipmentVariants(std::string_view variants_filename)
{
   std::vector<EquipmentVariant> equipment_variants;

   std::set<std::string> required_techs;
   std::set<std::string> blocking_techs;
   std::vector<std::pair<std::string, std::string>> text_items;

   commonItems::parser variant_parser;
   variant_parser.registerKeyword("required_techs", [&required_techs](std::istream& input_stream) {
      for (const std::string& tech: commonItems::getStrings(input_stream))
      {
         required_techs.insert(tech);
      }
   });
   variant_parser.registerKeyword("blocking_techs", [&blocking_techs](std::istream& input_stream) {
      for (const std::string& tech: commonItems::getStrings(input_stream))
      {
         blocking_techs.insert(tech);
      }
   });
   variant_parser.registerRegex(commonItems::catchallRegex,
       [&text_items](const std::string& key, std::istream& input_stream) {
          text_items.emplace_back(key, commonItems::stringOfItem(input_stream).getString());
       });

   commonItems::parser file_parser;
   file_parser.registerRegex(commonItems::catchallRegex,
       [&equipment_variants, &required_techs, &blocking_techs, &text_items, &variant_parser](const std::string& unused,
           std::istream& input_stream) {
          required_techs.clear();
          blocking_techs.clear();
          text_items.clear();

          variant_parser.parseStream(input_stream);

          equipment_variants.emplace_back(required_techs, blocking_techs, text_items);
       });

   file_parser.parseFile(variants_filename);

   return equipment_variants;
}