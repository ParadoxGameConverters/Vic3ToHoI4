#include "src/vic3_world/military/combat_unit_importer.h"

#include "external/commonItems/ParserHelpers.h"



namespace vic3
{

CombatUnitImporter::CombatUnitImporter()
{
   combat_unit_parser_.registerKeyword("current_manpower", [this](std::istream& stream) {
      current_manpower_ = commonItems::getInt(stream);
   });
   combat_unit_parser_.registerKeyword("type", [this](std::istream& stream) {
      type_ = commonItems::getString(stream);
   });
   combat_unit_parser_.registerKeyword("country", [this](std::istream& stream) {
      country_ = commonItems::getInt(stream);
   });
   combat_unit_parser_.registerKeyword("formation", [this](std::istream& stream) {
      formation_ = commonItems::getInt(stream);
   });
   combat_unit_parser_.IgnoreUnregisteredItems();
}


CombatUnit CombatUnitImporter::ImportCombatUnit(std::istream& stream)
{
   current_manpower_ = 0;
   type_.reset();
   country_.reset();
   formation_.reset();

   combat_unit_parser_.parseStream(stream);

   return {.current_manpower = current_manpower_, .type = type_, .country = country_, .formation = formation_};
}

}  // namespace vic3