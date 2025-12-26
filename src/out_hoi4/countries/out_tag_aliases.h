#pragma


#include <filesystem>
#include <set>

#include "src/hoi4_world/countries/tag_alias.h"


namespace out
{

void OutputTagAliases(const std::filesystem::path& output_name, const std::set<hoi4::TagAlias>& tag_aliases);

}  // namespace out