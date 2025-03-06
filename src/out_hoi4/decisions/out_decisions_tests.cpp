#include <external/commonItems/OSCompatibilityLayer.h>
#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <fstream>
#include <sstream>

#include "src/out_hoi4/decisions/out_decisions.h"



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
}

}  // namespace


namespace out
{

// Remove this test when decisions are implemented!
TEST(OutHoI4DecisionsOutDecisions, DecisionsAreOutput)
{
   const std::map<std::string, std::vector<hoi4::Decision>> decisions{
       {"$TAG$_a_decisions_category",
           std::vector<hoi4::Decision>{hoi4::Decision{.name = "$TAG$_a_decision"},
               hoi4::Decision{.name = "$TAG$_another_decision"}}},
       {"$TAG$_another_decisions_category",
           std::vector<hoi4::Decision>{hoi4::Decision{.name = "$TAG$_a_third_decision"}}},
   };

   CreateFolders("DecisionsAreOutput");
   OutputDecisions("DecisionsAreOutput", decisions);

   const std::filesystem::path expected_file = "output/DecisionsAreOutput/common/decisions/stories_decisions.txt";
   EXPECT_TRUE(commonItems::DoesFileExist(expected_file));
   std::ifstream decisions_file(expected_file);
   ASSERT_TRUE(decisions_file.is_open());
   std::stringstream decisions_file_stream;
   std::copy(std::istreambuf_iterator<char>(decisions_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(decisions_file_stream));
   decisions_file.close();
   EXPECT_EQ(decisions_file_stream.str(),
       "$TAG$_a_decisions_category = {\n"
       "\n"
       "\n"
       "\t$TAG$_a_decision = {\n"
       "\t}\n"
       "\n"
       "\t$TAG$_another_decision = {\n"
       "\t}\n"
       "\n"
       "}\n"
       "\n"
       "$TAG$_another_decisions_category = {\n"
       "\n"
       "\n"
       "\t$TAG$_a_third_decision = {\n"
       "\t}\n"
       "\n"
       "}\n"
       "\n");
}

}  // namespace out
