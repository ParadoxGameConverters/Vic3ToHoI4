#ifndef SRC_VIC3WORLD_MILITARY_MILITARYFORMATIONSIMPORTER_H
#define SRC_VIC3WORLD_MILITARY_MILITARYFORMATIONSIMPORTER_H


#include <istream>
#include <map>

#include "src/vic3_world/military/military_formation.h"
#include "src/vic3_world/military/military_formation_importer.h"



namespace vic3
{

std::map<int, MilitaryFormation> ImportMilitaryFormations(std::istream& input);

}  // namespace vic3



#endif  // SRC_VIC3WORLD_MILITARY_MILITARYFORMATIONSIMPORTER_H