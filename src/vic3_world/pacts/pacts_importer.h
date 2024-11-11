#ifndef SRC_VIC3WORLD_PACTS_PACTSIMPORTER_H
#define SRC_VIC3WORLD_PACTS_PACTSIMPORTER_H



#include <istream>
#include <map>

#include "src/vic3_world/pacts/pact.h"



namespace vic3
{

std::map<int, Pact> ImportPacts(std::istream& input_stream);

}  // namespace vic3



#endif  // SRC_VIC3WORLD_PACTS_PACTSIMPORTER_H