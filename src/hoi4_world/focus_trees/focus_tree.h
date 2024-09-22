#ifndef SRC_HOI4WORLD_NATIONALFOCUS_FOCUSTREE_H
#define SRC_HOI4WORLD_NATIONALFOCUS_FOCUSTREE_H



#include <string>
#include <vector>

#include "src/hoi4_world/focus_trees/focus.h"



namespace hoi4
{

struct FocusTree
{
   std::vector<std::string> shared_focuses;
   std::vector<Focus> focuses;

   std::strong_ordering operator<=>(const FocusTree&) const = default;
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_NATIONALFOCUS_FOCUSTREE_H