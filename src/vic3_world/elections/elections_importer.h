#ifndef SRC_VIC3WORLD_ELECTIONS_ELECTIONSIMPORTER_H
#define SRC_VIC3WORLD_ELECTIONS_ELECTIONSIMPORTER_H



#include <external/commonItems/Date.h>

#include <istream>
#include <map>
#include <string>



namespace vic3
{

std::map<int, date> ImportElections(std::istream& input_stream);

}  // namespace vic3



#endif  // SRC_VIC3WORLD_ELECTIONS_ELECTIONSIMPORTER_H