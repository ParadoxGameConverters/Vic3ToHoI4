#include "src/mappers/unit/unit_mapper_importer.h"

#include "external/commonItems/CommonRegexes.h"
#include "external/commonItems/Parser.h"
#include "external/commonItems/ParserHelpers.h"

mappers::UnitMapper mappers::ImportUnitMapper(std::string_view mapping_file)
{
   mappers::TemplateMap templates;
   mappers::BattalionTemplate current;

   commonItems::parser pm_parser;
   pm_parser.registerKeyword("equipment_scale", [&current](std::istream& input_stream) {
      current.equipment = commonItems::getInt(input_stream);
   });
   pm_parser.registerRegex(commonItems::catchallRegex, [&current](const std::string& key, std::istream& input_stream) {
      current.units.emplace(key, commonItems::getDouble(input_stream));
   });

   commonItems::parser file_parser;
   file_parser.registerRegex(commonItems::catchallRegex,
       [&pm_parser, &templates, &current](const std::string& production_method, std::istream& input_stream) {
          current.equipment = 0;
          current.units.clear();
          pm_parser.parseStream(input_stream);
          templates.emplace(production_method, current);
       });

   file_parser.parseFile(mapping_file);
   return mappers::UnitMapper(templates);
}
