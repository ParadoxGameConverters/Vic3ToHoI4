#pragma once



#include <filesystem>

#include "src/hoi4_world/countries/tag_alias.h"



namespace out
{

void OutputTagAlias(const hoi4::TagAlias& tag_alias, std::ofstream& tags_file);

}  // namespace out