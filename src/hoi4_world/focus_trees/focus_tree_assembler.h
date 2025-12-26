#ifndef SRC_HOI4WORLD_NATIONALFOCUS_FOCUSTREEASSEMBLER_H
#define SRC_HOI4WORLD_NATIONALFOCUS_FOCUSTREEASSEMBLER_H



#include <vector>

#include "src/hoi4_world/focus_trees/focus_tree.h"
#include "src/hoi4_world/roles/role.h"



namespace hoi4
{

// Creates a focus tree for the specified tag from the provided roles
FocusTree AssembleTree(const std::vector<Role>& roles,
    std::string_view tag,
    const std::map<std::string, std::string>& aliases,
    const hoi4::World& world);

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_NATIONALFOCUS_FOCUSTREEASSEMBLER_H