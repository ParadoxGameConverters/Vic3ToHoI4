#include "src/out_hoi4/decisions/out_decisions_categories.h"

#include <fstream>

#include "external/fmt/include/fmt/format.h"
#include "src/out_hoi4/decisions/out_decisions_category.h"



namespace out
{

void OutputDecisionsCategories(std::string_view output_name,
    const std::set<hoi4::DecisionsCategory>& decisions_categories)
{
   std::string categories_file_name =
       fmt::format("output/{}/common/decisions/categories/converter_decision_categories.txt", output_name);
   std::ofstream categories_file(categories_file_name);
   if (!categories_file.is_open())
   {
      throw std::runtime_error(fmt::format("Could not create {}", categories_file_name));
   }

   for (const hoi4::DecisionsCategory& decisions_category: decisions_categories)
   {
      categories_file << decisions_category;
   }

   categories_file.close();
}

}  // namespace out