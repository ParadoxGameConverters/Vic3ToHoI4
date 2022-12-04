#ifndef SRC_HOI4WORLD_MAP_PROVINCE_H
#define SRC_HOI4WORLD_MAP_PROVINCE_H



#include <compare>
#include <string>



namespace hoi4
{

struct Province
{
   bool is_land = false;
   std::string type;

   std::strong_ordering operator<=>(const Province&) const = default;
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_MAP_PROVINCE_H