#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/hoi4_world/roles/repeat_focus.h"
#include "src/hoi4_world/roles/repeat_focus_importer.h"



namespace hoi4
{

TEST(Hoi4worldRolesRepeatfocusimporterTests, DefaultsAreDefaulted)
{
   std::stringstream input;

   RepeatFocusImporter importer;
   const RepeatFocus repeat_focus = importer.ImportRepeatFocus(input);

   // requirement is handled in a currently-untestable way. When it is replaced, actually test it here.
   EXPECT_TRUE(repeat_focus.focuses.empty());
}


TEST(Hoi4worldRolesRepeatfocusimporterTests, ItemsCanBeImported)
{
   std::stringstream input;
   input << "focus = {\n";
   input << "\tid = focus_one\n";
   input << "}\n";
   input << "focus = {\n";
   input << "\tid = focus_two\n";
   input << "}\n";

   RepeatFocusImporter importer;
   const RepeatFocus repeat_focus = importer.ImportRepeatFocus(input);

   // requirement is handled in a currently-untestable way. When it is replaced, actually test it here.
   EXPECT_THAT(repeat_focus.focuses, testing::ElementsAre(Focus{.id = "focus_one"}, Focus{.id = "focus_two"}));
}

}  // namespace hoi4