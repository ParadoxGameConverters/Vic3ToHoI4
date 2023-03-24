#ifndef SRC_HOI4WORLD_MAP_RESOURCESMAP_H
#define SRC_HOI4WORLD_MAP_RESOURCESMAP_H



#include <map>
#include <string>



namespace hoi4
{

using Province = int;
using Resources = std::map<std::string, double>;
using ResourcesMap = std::map<Province, Resources>;

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_MAP_RESOURCESMAP_H