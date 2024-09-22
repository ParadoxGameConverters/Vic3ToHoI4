#ifndef SRC_OUTHOI4_FOCUSTREES_OUTFOCUS_H
#define SRC_OUTHOI4_FOCUSTREES_OUTFOCUS_H



#include <ostream>

#include "src/hoi4_world/focus_trees/focus.h"



namespace out
{

std::ostream& operator<<(std::ostream& output, const hoi4::Focus& focus);

}  // namespace out



#endif  // SRC_OUTHOI4_FOCUSTREES_OUTFOCUS_H