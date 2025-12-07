#include "src/out_hoi4/countries/out_tag_alias.h"

#include <print>



namespace out
{

void OutputTagAlias(const hoi4::TagAlias& tag_alias, std::ofstream& tags_file)
{
   std::println(tags_file, "{} = {{\n", tag_alias.alias);
   std::println(tags_file, "	original_tag = {}\n", tag_alias.original_tag);
   std::println(tags_file, "	has_country_flag = {}\n", tag_alias.flag);
   std::println(tags_file, "}}\n");
}

}  // namespace out