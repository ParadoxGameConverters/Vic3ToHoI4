#ifndef SRC_VIC3WORLD_BUILDINGS_BUILDINGS_IMPORTER_H
#define SRC_VIC3WORLD_BUILDINGS_BUILDINGS_IMPORTER_H



#include <string>

#include "src/vic3_world/buildings/buildings.h"



namespace vic3
{

Buildings ImportBuildings(std::istream& input_stream);

}  // namespace vic3



#endif  // SRC_VIC3WORLD_BUILDINGS_BUILDINGS_IMPORTER_H
