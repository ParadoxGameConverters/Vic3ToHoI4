#include "src/vic3_world/military/combat_units_importer.h"

#include "external/commonItems/CommonRegexes.h"
#include "src/vic3_world/database/database_parser.h"
#include "src/vic3_world/military/combat_unit_importer.h"



namespace vic3
{

std::vector<CombatUnit> ImportCombatUnits(std::istream& input)
{
   std::vector<vic3::CombatUnit> combat_units;

   CombatUnitImporter unit_importer;

   const auto& parser_function = [&unit_importer, &combat_units]([[maybe_unused]] const std::string& number_string,
                                     std::istream& input_stream) {
      combat_units.push_back(unit_importer.ImportCombatUnit(input_stream));
   };

   DatabaseParser parser(parser_function);
   parser.registerRegex(commonItems::integerRegex, parser_function);
   parser.parseStream(input);

   return combat_units;
}

}  // namespace vic3