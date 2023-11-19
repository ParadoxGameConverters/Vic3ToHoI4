#ifndef SRC_VIC3WORLD_WARS_WARSIMPORTER_H
#define SRC_VIC3WORLD_WARS_WARSIMPORTER_H



#include <istream>
#include <vector>

#include "src/vic3_world/wars/war.h"



namespace vic3
{

std::vector<War> ImportWars(std::istream& input_stream);

}  // namespace vic3



#endif  // SRC_VIC3WORLD_WARS_WARSIMPORTER_H