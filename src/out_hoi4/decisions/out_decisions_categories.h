#ifndef OUT_DECISIONS_CATEGORIES_H
#define OUT_DECISIONS_CATEGORIES_H



#include <set>

#include "src/hoi4_world/decisions/decisions_category.h"



namespace out
{

void OutputDecisionsCategories(std::string_view output_name,
    const std::set<hoi4::DecisionsCategory>& decisions_categories);

}  // namespace out



#endif  // OUT_DECISIONS_CATEGORIES_H