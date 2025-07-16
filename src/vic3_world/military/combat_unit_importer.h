#pragma once



#include "external/commonItems/parser.h"
#include "src/vic3_world/military/combat_unit.h"



namespace vic3
{

class CombatUnitImporter
{
  public:
   CombatUnitImporter();
   CombatUnit ImportCombatUnit(std::istream& stream);

  private:
   commonItems::parser combat_unit_parser_;

   int current_manpower = 0;
   std::string type;
   int country;
   int formation;
};

}  // namespace vic3