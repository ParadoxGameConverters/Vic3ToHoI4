#include "src/out_hoi4/decisions/out_decisions_categories.h"

#include <external/fmt/include/fmt/format.h>

#include <fstream>

#include "src/out_hoi4/decisions/out_decisions_category.h"



using std::filesystem::path;



namespace out
{

void OutputDecisionsCategories(const path& output_name, const std::set<hoi4::DecisionsCategory>& decisions_categories)
{
   const path categories_file_name =
       "output" / output_name / "common/decisions/categories/converter_decision_categories.txt";
   std::ofstream categories_file(categories_file_name);
   if (!categories_file.is_open())
   {
      throw std::runtime_error(fmt::format("Could not create {}", categories_file_name.string()));
   }

   for (const hoi4::DecisionsCategory& decisions_category: decisions_categories)
   {
      categories_file << decisions_category;
   }

   categories_file.close();
}

}  // namespace out