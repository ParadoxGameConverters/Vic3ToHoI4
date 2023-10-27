#ifndef SRC_HOI4WORLD_MILITARY_SHIP_H
#define SRC_HOI4WORLD_MILITARY_SHIP_H

#include <string>

namespace hoi4
{

class Ship
{
  public:
   Ship() {}

   std::partial_ordering operator<=>(const Ship&) const = default;
  private:
};

}  // namespace hoi4


#endif  // SRC_HOI4WORLD_MILITARY_SHIP_H
