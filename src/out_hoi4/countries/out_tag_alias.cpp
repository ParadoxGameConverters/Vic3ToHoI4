#include "src/out_hoi4/countries/out_tag_alias.h"

#include <print>



namespace out
{

void OutputTagAlias(const hoi4::TagAlias& tag_alias, std::ostream& tags_file)
{
   std::println(tags_file, "{} = {{", tag_alias.alias);
   std::println(tags_file, "\toriginal_tag = {}", tag_alias.original_tag);
   std::println(tags_file, "\thas_country_flag = {}", tag_alias.flag);
   std::println(tags_file, "}}");
   std::println(tags_file, "");
}

}  // namespace out