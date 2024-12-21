#include <ostream>
#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/out_hoi4/decisions/out_decisions_category.h"



namespace out
{

TEST(Outhoi4DecisionsOutdecisionscategory, BasicsAreOutput)
{
   const hoi4::DecisionsCategory category({.name = "test_name"});

   std::ostringstream output;
   output << category;

   std::ostringstream expected;
   expected << "test_name = {\n";
   expected << "}\n";
   expected << "\n";
   EXPECT_EQ(output.str(), expected.str());
}


TEST(Outhoi4DecisionsOutdecisionscategory, SetItemsAreOutput)
{
   const hoi4::DecisionsCategory category({
       .name = "test_name",
       .icon = "test_icon",
       .picture = "test_picture",
       .priority = 42,
       .allowed = "= { test_allowed }",
       .visible = "= { test_visible }",
       .visibility_type = "= { test_visibility_type }",
       .extra_items =
           {
               {"extra_one", "= { value_one }"},
               {"extra_two", "= { value_two }"},
               {"extra_two", "= { value_three }"},
           },
   });

   std::ostringstream output;
   output << category;

   std::ostringstream expected;
   expected << "test_name = {\n";
   expected << "\ticon = test_icon\n";
   expected << "\tpicture = test_picture\n";
   expected << "\tpriority = 42\n";
   expected << "\tallowed = { test_allowed }\n";
   expected << "\tvisible = { test_visible }\n";
   expected << "\tvisibility_type = { test_visibility_type }\n";
   expected << "\textra_one = { value_one }\n";
   expected << "\textra_two = { value_two }\n";
   expected << "\textra_two = { value_three }\n";
   expected << "}\n";
   expected << "\n";
   EXPECT_EQ(output.str(), expected.str());
}

}  // namespace out
