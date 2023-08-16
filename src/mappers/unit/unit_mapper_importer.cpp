#include "src/mappers/unit/unit_mapper_importer.h"

#include "external/commonItems/CommonRegexes.h"
#include "external/commonItems/Parser.h"
#include "external/commonItems/ParserHelpers.h"



mappers::UnitMapper mappers::ImportUnitMapper(std::string_view mapping_file)
{
   PmToPointsMap methods;
   UnitPointsMap unit_points_map;

   commonItems::parser pm_parser;
   pm_parser.registerRegex(commonItems::catchallRegex,
       [&unit_points_map](const std::string& unit, std::istream& input_stream) {
          unit_points_map.emplace(unit, commonItems::getDouble(input_stream));
       });

   commonItems::parser file_parser;
   file_parser.registerRegex(commonItems::catchallRegex,
       [&pm_parser, &methods, &unit_points_map](const std::string& production_method, std::istream& input_stream) {
          unit_points_map.clear();
          pm_parser.parseStream(input_stream);
          methods.emplace(production_method, unit_points_map);
       });

   file_parser.parseFile(mapping_file);
   return UnitMapper(methods);
}
