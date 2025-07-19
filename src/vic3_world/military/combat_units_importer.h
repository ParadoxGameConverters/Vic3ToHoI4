#pragma once



#include <istream>
#include <vector>

#include "src/vic3_world/military/combat_unit.h"



namespace vic3
{

std::vector<CombatUnit> ImportCombatUnits(std::istream& input);

}  // namespace vic3