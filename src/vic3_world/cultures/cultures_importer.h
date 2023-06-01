#ifndef SRC_VIC3WORLD_CULTURES_CULTURESIMPORTER_H
#define SRC_VIC3WORLD_CULTURES_CULTURESIMPORTER_H



#include <istream>
#include <map>
#include <set>
#include <string>



namespace vic3
{

std::map<int, std::string> ImportCultures(std::istream& input_stream);

}  // namespace vic3



#endif  // SRC_VIC3WORLD_CULTURES_CULTURESIMPORTER_H