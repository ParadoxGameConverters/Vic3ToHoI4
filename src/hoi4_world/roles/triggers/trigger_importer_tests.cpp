#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

#include "src/hoi4_world/roles/triggers/always_trigger.h"
#include "src/hoi4_world/roles/triggers/and_trigger.h"
#include "src/hoi4_world/roles/triggers/any_other_country_trigger.h"
#include "src/hoi4_world/roles/triggers/is_capital_trigger.h"
#include "src/hoi4_world/roles/triggers/is_on_continent_trigger.h"
#include "src/hoi4_world/roles/triggers/not_trigger.h"
#include "src/hoi4_world/roles/triggers/or_trigger.h"
#include "src/hoi4_world/roles/triggers/tag_trigger.h"
#include "src/hoi4_world/roles/triggers/trigger_importer.h"



namespace hoi4
{

// trigger scopes
TEST(Hoi4worldRolesTriggersTriggerimporterTests, AnyOtherCountryTriggerCanBeImported)
{
   std::stringstream input;
   input << "= {\n";
   input << "  any_other_country = {\n";
   input << "    always = yes\n";
   input << "    always = no\n";
   input << "    tag = TAG\n";
   input << "  }";
   input << "}";

   TriggerImporter importer;
   const std::unique_ptr<Trigger> trigger = importer.ImportTrigger(input);

   ASSERT_TRUE(trigger);

   std::unique_ptr<Trigger> always_yes_trigger = std::make_unique<AlwaysTrigger>(true);
   std::unique_ptr<Trigger> always_no_trigger = std::make_unique<AlwaysTrigger>(false);
   std::unique_ptr<Trigger> tag_trigger = std::make_unique<TagTrigger>("TAG");
   std::vector<std::unique_ptr<Trigger>> children;
   children.push_back(std::move(always_yes_trigger));
   children.push_back(std::move(always_no_trigger));
   children.push_back(std::move(tag_trigger));
   const AnyOtherCountryTrigger and_trigger(std::move(children));

   EXPECT_EQ(*trigger, and_trigger);
}

// dual scopes
TEST(Hoi4worldRolesTriggersTriggerimporterTests, TagTriggerCanBeImported)
{
   std::stringstream input;
   input << "= {\n";
   input << "  tag = TAG\n";
   input << "}";

   TriggerImporter importer;
   const std::unique_ptr<Trigger> trigger = importer.ImportTrigger(input);

   ASSERT_TRUE(trigger);
   const TagTrigger tag_trigger("TAG");
   EXPECT_EQ(*trigger, tag_trigger);
}


// flow control tools
TEST(Hoi4worldRolesTriggersTriggerimporterTests, AndTriggerCanBeImported)
{
   std::stringstream input;
   input << "= {\n";
   input << "  AND = {\n";
   input << "    always = yes\n";
   input << "    always = no\n";
   input << "    tag = TAG\n";
   input << "  }";
   input << "}";

   TriggerImporter importer;
   const std::unique_ptr<Trigger> trigger = importer.ImportTrigger(input);

   ASSERT_TRUE(trigger);

   std::unique_ptr<Trigger> always_yes_trigger = std::make_unique<AlwaysTrigger>(true);
   std::unique_ptr<Trigger> always_no_trigger = std::make_unique<AlwaysTrigger>(false);
   std::unique_ptr<Trigger> tag_trigger = std::make_unique<TagTrigger>("TAG");
   std::vector<std::unique_ptr<Trigger>> children;
   children.push_back(std::move(always_yes_trigger));
   children.push_back(std::move(always_no_trigger));
   children.push_back(std::move(tag_trigger));
   const AndTrigger and_trigger(std::move(children));

   EXPECT_EQ(*trigger, and_trigger);
}


TEST(Hoi4worldRolesTriggersTriggerimporterTests, LowercaseAndTriggerCanBeImported)
{
	std::stringstream input;
	input << "= {\n";
	input << "  and = {\n";
	input << "    always = yes\n";
	input << "    always = no\n";
	input << "    tag = TAG\n";
	input << "  }";
	input << "}";

	TriggerImporter importer;
	const std::unique_ptr<Trigger> trigger = importer.ImportTrigger(input);

	ASSERT_TRUE(trigger);

	std::unique_ptr<Trigger> always_yes_trigger = std::make_unique<AlwaysTrigger>(true);
	std::unique_ptr<Trigger> always_no_trigger = std::make_unique<AlwaysTrigger>(false);
	std::unique_ptr<Trigger> tag_trigger = std::make_unique<TagTrigger>("TAG");
	std::vector<std::unique_ptr<Trigger>> children;
	children.push_back(std::move(always_yes_trigger));
	children.push_back(std::move(always_no_trigger));
	children.push_back(std::move(tag_trigger));
	const AndTrigger and_trigger(std::move(children));

	EXPECT_EQ(*trigger, and_trigger);
}


TEST(Hoi4worldRolesTriggersTriggerimporterTests, MultipleTopLevelTriggersCountAsAnAndTrigger)
{
   std::stringstream input;
   input << "= {\n";
   input << "  always = yes\n";
   input << "  always = no\n";
   input << "  tag = TAG\n";
   input << "}";

   TriggerImporter importer;
   const std::unique_ptr<Trigger> trigger = importer.ImportTrigger(input);

   ASSERT_TRUE(trigger);

   std::unique_ptr<Trigger> always_yes_trigger = std::make_unique<AlwaysTrigger>(true);
   std::unique_ptr<Trigger> always_no_trigger = std::make_unique<AlwaysTrigger>(false);
   std::unique_ptr<Trigger> tag_trigger = std::make_unique<TagTrigger>("TAG");
   std::vector<std::unique_ptr<Trigger>> children;
   children.push_back(std::move(always_yes_trigger));
   children.push_back(std::move(always_no_trigger));
   children.push_back(std::move(tag_trigger));
   const AndTrigger and_trigger(std::move(children));

   EXPECT_EQ(*trigger, and_trigger);
}


TEST(Hoi4worldRolesTriggersTriggerimporterTests, OrTriggerCanBeImported)
{
   std::stringstream input;
   input << "= {\n";
   input << "  OR = {\n";
   input << "    always = yes\n";
   input << "    always = no\n";
   input << "    tag = TAG\n";
   input << "  }";
   input << "}";

   TriggerImporter importer;
   const std::unique_ptr<Trigger> trigger = importer.ImportTrigger(input);

   ASSERT_TRUE(trigger);

   std::unique_ptr<Trigger> always_yes_trigger = std::make_unique<AlwaysTrigger>(true);
   std::unique_ptr<Trigger> always_no_trigger = std::make_unique<AlwaysTrigger>(false);
   std::unique_ptr<Trigger> tag_trigger = std::make_unique<TagTrigger>("TAG");
   std::vector<std::unique_ptr<Trigger>> children;
   children.push_back(std::move(always_yes_trigger));
   children.push_back(std::move(always_no_trigger));
   children.push_back(std::move(tag_trigger));
   const OrTrigger or_trigger(std::move(children));

   EXPECT_EQ(*trigger, or_trigger);
}


TEST(Hoi4worldRolesTriggersTriggerimporterTests, LowercaseOrTriggerCanBeImported)
{
	std::stringstream input;
	input << "= {\n";
	input << "  or = {\n";
	input << "    always = yes\n";
	input << "    always = no\n";
	input << "    tag = TAG\n";
	input << "  }";
	input << "}";

	TriggerImporter importer;
	const std::unique_ptr<Trigger> trigger = importer.ImportTrigger(input);

	ASSERT_TRUE(trigger);

	std::unique_ptr<Trigger> always_yes_trigger = std::make_unique<AlwaysTrigger>(true);
	std::unique_ptr<Trigger> always_no_trigger = std::make_unique<AlwaysTrigger>(false);
	std::unique_ptr<Trigger> tag_trigger = std::make_unique<TagTrigger>("TAG");
	std::vector<std::unique_ptr<Trigger>> children;
	children.push_back(std::move(always_yes_trigger));
	children.push_back(std::move(always_no_trigger));
	children.push_back(std::move(tag_trigger));
	const OrTrigger or_trigger(std::move(children));

	EXPECT_EQ(*trigger, or_trigger);
}


TEST(Hoi4worldRolesTriggersTriggerimporterTests, NotTriggerCanBeImported)
{
   std::stringstream input;
   input << "= {\n";
   input << "  NOT = {\n";
   input << "    always = yes\n";
   input << "    always = no\n";
   input << "    tag = TAG\n";
   input << "  }";
   input << "}";

   TriggerImporter importer;
   const std::unique_ptr<Trigger> trigger = importer.ImportTrigger(input);

   ASSERT_TRUE(trigger);

   std::unique_ptr<Trigger> always_yes_trigger = std::make_unique<AlwaysTrigger>(true);
   std::unique_ptr<Trigger> always_no_trigger = std::make_unique<AlwaysTrigger>(false);
   std::unique_ptr<Trigger> tag_trigger = std::make_unique<TagTrigger>("TAG");
   std::vector<std::unique_ptr<Trigger>> children;
   children.push_back(std::move(always_yes_trigger));
   children.push_back(std::move(always_no_trigger));
   children.push_back(std::move(tag_trigger));
   const NotTrigger not_trigger(std::move(children));

   EXPECT_EQ(*trigger, not_trigger);
}


TEST(Hoi4worldRolesTriggersTriggerimporterTests, LowercaseNotTriggerCanBeImported)
{
	std::stringstream input;
	input << "= {\n";
	input << "  NOT = {\n";
	input << "    always = yes\n";
	input << "    always = no\n";
	input << "    tag = TAG\n";
	input << "  }";
	input << "}";

	TriggerImporter importer;
	const std::unique_ptr<Trigger> trigger = importer.ImportTrigger(input);

	ASSERT_TRUE(trigger);

	std::unique_ptr<Trigger> always_yes_trigger = std::make_unique<AlwaysTrigger>(true);
	std::unique_ptr<Trigger> always_no_trigger = std::make_unique<AlwaysTrigger>(false);
	std::unique_ptr<Trigger> tag_trigger = std::make_unique<TagTrigger>("TAG");
	std::vector<std::unique_ptr<Trigger>> children;
	children.push_back(std::move(always_yes_trigger));
	children.push_back(std::move(always_no_trigger));
	children.push_back(std::move(tag_trigger));
	const NotTrigger not_trigger(std::move(children));

	EXPECT_EQ(*trigger, not_trigger);
}


// any scope
TEST(Hoi4worldRolesTriggersTriggerimporterTests, EmptyInputGivesAlwaysYesTrigger)
{
   std::stringstream input;

   TriggerImporter importer;
   const std::unique_ptr<Trigger> trigger = importer.ImportTrigger(input);

   ASSERT_TRUE(trigger);
   const AlwaysTrigger always_trigger(true);
   EXPECT_EQ(*trigger, always_trigger);
}


TEST(Hoi4worldRolesTriggersTriggerimporterTests, AlwaysTriggerCanBeImportedWithYes)
{
   std::stringstream input;
   input << "= {\n";
   input << "  always = yes\n";
   input << "}";

   TriggerImporter importer;
   const std::unique_ptr<Trigger> trigger = importer.ImportTrigger(input);

   ASSERT_TRUE(trigger);
   const AlwaysTrigger always_trigger(true);
   EXPECT_EQ(*trigger, always_trigger);
}


TEST(Hoi4worldRolesTriggersTriggerimporterTests, AlwaysTriggerCanBeImportedWithNo)
{
   std::stringstream input;
   input << "= {\n";
   input << "  always = no\n";
   input << "}";

   TriggerImporter importer;
   const std::unique_ptr<Trigger> trigger = importer.ImportTrigger(input);

   ASSERT_TRUE(trigger);
   const AlwaysTrigger always_trigger(false);
   EXPECT_EQ(*trigger, always_trigger);
}


TEST(Hoi4worldRolesTriggersTriggerimporterTests, AlwaysTriggerCanBeImportedWithLoudYes)
{
   std::stringstream input;
   input << "= {\n";
   input << "  always = YES\n";
   input << "}";

   TriggerImporter importer;
   const std::unique_ptr<Trigger> trigger = importer.ImportTrigger(input);

   ASSERT_TRUE(trigger);
   const AlwaysTrigger always_trigger(true);
   EXPECT_EQ(*trigger, always_trigger);
}


// state scope
TEST(Hoi4worldRolesTriggersTriggerimporterTests, IsCapitalTriggerCanBeImportedWithYes)
{
   std::stringstream input;
   input << "= {\n";
   input << "  is_capital = yes\n";
   input << "}";

   TriggerImporter importer;
   const std::unique_ptr<Trigger> trigger = importer.ImportTrigger(input);

   ASSERT_TRUE(trigger);
   const IsCapitalTrigger is_capital_trigger(true);
   EXPECT_EQ(*trigger, is_capital_trigger);
}


TEST(Hoi4worldRolesTriggersTriggerimporterTests, IsCapitalTriggerCanBeImportedWithNo)
{
   std::stringstream input;
   input << "= {\n";
   input << "  is_capital = no\n";
   input << "}";

   TriggerImporter importer;
   const std::unique_ptr<Trigger> trigger = importer.ImportTrigger(input);

   ASSERT_TRUE(trigger);
   const IsCapitalTrigger is_capital_trigger(false);
   EXPECT_EQ(*trigger, is_capital_trigger);
}


TEST(Hoi4worldRolesTriggersTriggerimporterTests, IsCapitalTriggerCanBeImportedWithLoudYes)
{
   std::stringstream input;
   input << "= {\n";
   input << "  is_capital = YES\n";
   input << "}";

   TriggerImporter importer;
   const std::unique_ptr<Trigger> trigger = importer.ImportTrigger(input);

   ASSERT_TRUE(trigger);
   const IsCapitalTrigger is_capital_trigger(true);
   EXPECT_EQ(*trigger, is_capital_trigger);
}


TEST(Hoi4worldRolesTriggersTriggerimporterTests, IsOnContinentTriggerCanBeImported)
{
   std::stringstream input;
   input << "= {\n";
   input << "  is_on_continent = test_continent\n";
   input << "}";

   TriggerImporter importer;
   const std::unique_ptr<Trigger> trigger = importer.ImportTrigger(input);

   ASSERT_TRUE(trigger);
   const IsOnContinentTrigger is_on_continent_trigger("test_continent");
   EXPECT_EQ(*trigger, is_on_continent_trigger);
}

}  // namespace hoi4