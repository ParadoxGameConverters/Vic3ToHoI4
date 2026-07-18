#ifndef SRC_HOI4WORLD_MILITARY_SHIP_H
#define SRC_HOI4WORLD_MILITARY_SHIP_H



#include <string>



namespace hoi4
{

struct Ship
{
   std::string name;
   std::string definition;
   std::string equipment;
   std::string legacy_equipment;
   std::string version;

   std::strong_ordering operator<=>(const Ship&) const = default;
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_MILITARY_SHIP_H
