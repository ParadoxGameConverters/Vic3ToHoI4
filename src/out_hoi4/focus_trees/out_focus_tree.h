#ifndef SRC_OUTHOI4_NATIONALFOCUS_OUTFOCUSTREE_H
#define SRC_OUTHOI4_NATIONALFOCUS_OUTFOCUSTREE_H



#include <string_view>

#include "src/configuration/configuration.h"
#include "src/hoi4_world/focus_trees/focus_tree.h"



namespace out
{

void OutputFocusTree(std::string_view output_name,
    std::string_view tag,
    const hoi4::FocusTree& focus_tree,
    configuration::UseStories use_stories);

}  // namespace out



#endif  // SRC_OUTHOI4_NATIONALFOCUS_OUTFOCUSTREE_H