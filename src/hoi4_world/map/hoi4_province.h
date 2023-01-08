#ifndef SRC_HOI4WORLD_MAP_PROVINCE_H
#define SRC_HOI4WORLD_MAP_PROVINCE_H



#include <string>



namespace hoi4
{

struct Province
{
   bool is_land = false;
   std::string type;
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_MAP_PROVINCE_H