#include <filesystem>
#include <fstream>
#include <sstream>

#include "external/commonItems/OSCompatibilityLayer.h"
#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "external/fmt/include/fmt/format.h"
#include "src/hoi4_world/decisions/decisions_category.h"
#include "src/out_hoi4/decisions/out_decisions_categories.h"



namespace
{

void CreateFolders(std::string_view test_name)
{
   commonItems::TryCreateFolder("output");
   commonItems::TryCreateFolder(fmt::format("output/{}", test_name));
   commonItems::TryCreateFolder(fmt::format("output/{}/common", test_name));
   commonItems::TryCreateFolder(fmt::format("output/{}/common/decisions", test_name));
   commonItems::TryCreateFolder(fmt::format("output/{}/common/decisions/categories", test_name));
}

}  // namespace



namespace out
{

TEST(Outhoi4DecisionsOutdecisionscategoriesTests, CategoriesFileIsCreated)
{
   CreateFolders("CategoriesFileIsCreated");

   OutputDecisionsCategories("CategoriesFileIsCreated",
       {
           {.name = "category_one"},
           {.name = "category_two"},
       });

   EXPECT_TRUE(commonItems::DoesFileExist(
       "output/CategoriesFileIsCreated/common/decisions/categories/converter_decision_categories.txt"));
   std::ifstream categories_file(
       "output/CategoriesFileIsCreated/common/decisions/categories/converter_decision_categories.txt");
   ASSERT_TRUE(categories_file.is_open());
   std::stringstream categories_file_stream;
   std::copy(std::istreambuf_iterator<char>(categories_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(categories_file_stream));
   categories_file.close();

   EXPECT_EQ(categories_file_stream.str(),
       "category_one = {\n"
       "}\n"
       "\n"
       "category_two = {\n"
       "}\n"
       "\n");
}

}  // namespace out