#ifndef OUT_DECISIONS_CATEGORY_H
#define OUT_DECISIONS_CATEGORY_H



#include <ostream>

#include "src/hoi4_world/decisions/decisions_category.h"



namespace out
{

std::ostream& operator<<(std::ostream& out, const hoi4::DecisionsCategory& decisions_category);

}  // namespace out



#endif  // OUT_DECISIONS_CATEGORY_H