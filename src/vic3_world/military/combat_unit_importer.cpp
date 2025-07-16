#include "src/vic3_world/military/combat_unit_importer.h"

#include "external/commonItems/ParserHelpers.h"



namespace vic3
{

CombatUnitImporter::CombatUnitImporter()
{
   combat_unit_parser_.registerKeyword("current_manpower", [this](std::istream& stream) {
      current_manpower = commonItems::getInt(stream);
   });
   combat_unit_parser_.registerKeyword("type", [this](std::istream& stream) {
      type = commonItems::getString(stream);
   });
   combat_unit_parser_.registerKeyword("country", [this](std::istream& stream) {
      country = commonItems::getInt(stream);
   });
   combat_unit_parser_.registerKeyword("formation", [this](std::istream& stream) {
      formation = commonItems::getInt(stream);
   });
}


CombatUnit CombatUnitImporter::ImportCombatUnit(std::istream& stream)
{
   current_manpower = 0;
   type.clear();
   country = 0;
   formation = 0;

   combat_unit_parser_.parseStream(stream);

   return {.current_manpower = current_manpower, .type = type, .country = country, .formation = formation};
}

}  // namespace vic3