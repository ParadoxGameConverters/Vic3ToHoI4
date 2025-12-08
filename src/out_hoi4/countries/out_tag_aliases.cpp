#include "src/out_hoi4/countries/out_tag_aliases.h"

#include <external/fmt/include/fmt/format.h>

#include <fstream>

#include "out_tag_alias.h"



namespace out
{

void OutputTagAliases(const std::filesystem::path& output_name, const std::set<hoi4::TagAlias>& tag_aliases)
{
   if (!std::filesystem::create_directories("output" / output_name / "common/country_tag_aliases"))
   {
      throw std::runtime_error(
          fmt::format("Could not create output/{}/common/country_tag_aliases/", output_name.string()));
   }
   std::ofstream tags_file("output" / output_name / "common/country_tag_aliases/converter_tag_aliases.txt");
   if (!tags_file.is_open())
   {
      throw std::runtime_error(
          fmt::format("Could not open output/{}/common/country_tag_aliases/converter_tag_aliases.txt",
              output_name.string()));
   }

   for (const auto& tag_alias: tag_aliases)
   {
      OutputTagAlias(tag_alias, tags_file);
   }

   tags_file.close();
}

}  // namespace out