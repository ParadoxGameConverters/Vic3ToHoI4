#ifndef SRC_HOI4WORLD_ROLES_REPEATFOCUS_H
#define SRC_HOI4WORLD_ROLES_REPEATFOCUS_H



#include <string>
#include <vector>

#include "src/hoi4_world/focus_trees/focus.h"
#include "src/hoi4_world/world/hoi4_world.h"



namespace hoi4
{

struct RepeatFocus
{
   std::function<bool(const Country&, const World&)> requirement;
   std::vector<Focus> focuses;

   // operator<=> doesn't work for unknown reasons. When requirement is done properly and test are updated, try
   // replacing this with operator <=>
   bool operator==(const RepeatFocus& b) const { return focuses == b.focuses; }
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_ROLES_REPEATFOCUS_H