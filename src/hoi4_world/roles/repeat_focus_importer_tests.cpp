#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

#include "src/hoi4_world/roles/repeat_focus.h"
#include "src/hoi4_world/roles/repeat_focus_importer.h"
#include "src/hoi4_world/roles/requirements/always_trigger.h"
#include "src/hoi4_world/roles/requirements/and_trigger.h"
#include "src/hoi4_world/roles/requirements/tag_trigger.h"



namespace hoi4
{

TEST(Hoi4worldRolesRepeatfocusimporterTests, DefaultsAreDefaulted)
{
   std::stringstream input;

   RepeatFocusImporter importer;
   const RepeatFocus repeat_focus = importer.ImportRepeatFocus(input);

   // requirement is handled in a currently-untestable way. When it is replaced, actually test it here.
   EXPECT_TRUE(repeat_focus.GetFocuses().empty());
}


TEST(Hoi4worldRolesRepeatfocusimporterTests, ItemsCanBeImported)
{
   std::stringstream input;
   input << "requirement = {\n";
   input << "\ttag = TAG\n";
   input << "\talways = yes\n";
   input << "}\n";
   input << "focus = {\n";
   input << "\tid = focus_one\n";
   input << "}\n";
   input << "focus = {\n";
   input << "\tid = focus_two\n";
   input << "}\n";

   RepeatFocusImporter importer;
   const RepeatFocus repeat_focus = importer.ImportRepeatFocus(input);

   std::unique_ptr<Trigger> tag_trigger = std::make_unique<TagTrigger>("TAG");
   std::unique_ptr<Trigger> always_yes_trigger = std::make_unique<AlwaysTrigger>(true);
   std::vector<std::unique_ptr<Trigger>> children;
   children.push_back(std::move(tag_trigger));
   children.push_back(std::move(always_yes_trigger));
   const AndTrigger and_trigger(std::move(children));
   EXPECT_EQ(repeat_focus.GetRequirement(), and_trigger);

   EXPECT_THAT(repeat_focus.GetFocuses(), testing::ElementsAre(Focus{.id = "focus_one"}, Focus{.id = "focus_two"}));
}

}  // namespace hoi4