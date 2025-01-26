#include <external/commonItems/OSCompatibilityLayer.h>
#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>
#include <external/fmt/include/fmt/format.h>

#include <filesystem>
#include <fstream>
#include <sstream>

#include "src/hoi4_world/decisions/decisions_category.h"
#include "src/out_hoi4/decisions/out_decisions_categories.h"



using std::filesystem::create_directories;
using std::filesystem::path;



namespace
{

void CreateFolders(std::string_view test_name)
{
   const path test_path(test_name);
   create_directories("output");
   create_directories("output" / test_path);
   create_directories("output" / test_path / "common");
   create_directories("output" / test_path / "common/decisions");
   create_directories("output" / test_path / "common/decisions/categories");
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

   const path output_file(
       "output/CategoriesFileIsCreated/common/decisions/categories/converter_decision_categories.txt");
   EXPECT_TRUE(commonItems::DoesFileExist(output_file));
   std::ifstream categories_file(output_file);
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