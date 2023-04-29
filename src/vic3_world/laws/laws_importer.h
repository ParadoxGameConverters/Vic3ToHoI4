#ifndef SRC_VIC3WORLD_LAWS_LAWSIMPORTER_H
#define SRC_VIC3WORLD_LAWS_LAWSIMPORTER_H



#include <istream>
#include <map>
#include <set>
#include <string>



namespace vic3
{

std::map<int, std::set<std::string>> ImportLaws(std::istream& input_stream);

}  // namespace vic3



#endif  // SRC_VIC3WORLD_LAWS_LAWSIMPORTER_H