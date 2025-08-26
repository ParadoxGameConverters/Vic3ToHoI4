#ifndef SRC_VIC3WORLD_CULTURES_CULTURE_H
#define SRC_VIC3WORLD_CULTURES_CULTURE_H



#include <set>
#include <string>



namespace vic3
{

struct Culture
{
   std::string name;
   std::set<std::string> homelands;

   bool operator==(const Culture&) const = default;
};

}  // namespace vic3



#endif  // SRC_VIC3WORLD_CULTURES_CULTURE_H