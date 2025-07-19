#pragma once



#include "external/commonItems/Parser.h"
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

   int current_manpower_ = 0;
   std::optional<std::string> type_;
   std::optional<int> country_;
   std::optional<int> formation_;
};

}  // namespace vic3