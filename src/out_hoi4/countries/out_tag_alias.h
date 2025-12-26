#pragma once



#include <filesystem>
#include <ostream>

#include "src/hoi4_world/countries/tag_alias.h"



namespace out
{

void OutputTagAlias(const hoi4::TagAlias& tag_alias, std::ostream& tags_file);

}  // namespace out