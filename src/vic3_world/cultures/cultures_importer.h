#ifndef SRC_VIC3WORLD_CULTURES_CULTURESIMPORTER_H
#define SRC_VIC3WORLD_CULTURES_CULTURESIMPORTER_H



#include <istream>
#include <map>
#include <set>
#include <string>

#include "src/vic3_world/cultures/culture.h"



namespace vic3
{

std::map<int, Culture> ImportCultures(std::istream& input_stream);

}  // namespace vic3



#endif  // SRC_VIC3WORLD_CULTURES_CULTURESIMPORTER_H