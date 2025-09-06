#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

#include "src/vic3_world/wars/war_importer.h"



namespace vic3
{

TEST(Vic3WorldWarImporter, WarCanBeImported)
{
   WarImporter importer;
   std::stringstream input;
   input << R"(
{
	initiator=144
	target=169
	war=196
}
)";
   std::optional<War> war = importer.ImportWar(input);
   EXPECT_TRUE(war.has_value());
   EXPECT_EQ(war.value_or(War({})),
       War({
           .original_attacker = 144,
           .attackers = {144},
           .original_defender = 169,
           .defenders = {169},
       }));
}


TEST(Vic3WorldWarImporter, MissingWarNumberGivesNullopt)
{
   WarImporter importer;
   std::stringstream input;
   input << R""(
{
	initiator=144
	target=169
}
)"";
   std::optional<War> war = importer.ImportWar(input);
   EXPECT_FALSE(war.has_value());
}


TEST(Vic3WorldWarImporter, InvalidWarNumberGivesNullopt)
{
   WarImporter importer;
   std::stringstream input;
   input << R""(
{
	initiator=144
	target=169
	war=4294967295
}
)"";
   std::optional<War> war = importer.ImportWar(input);
   EXPECT_FALSE(war.has_value());
}


TEST(Vic3WorldWarImporter, MissingInitiatorGivesNullopt)
{
   WarImporter importer;
   std::stringstream input;
   input << R"(
{
	target=169
	war=196
}
)";
   std::optional<War> war = importer.ImportWar(input);
   EXPECT_FALSE(war.has_value());
}


TEST(Vic3WorldWarImporter, MissingTargetGivesNullopt)
{
   WarImporter importer;
   std::stringstream input;
   input << R"(
{
	initiator=144
	war=196
}
)";
   std::optional<War> war = importer.ImportWar(input);
   EXPECT_FALSE(war.has_value());
}


TEST(Vic3WorldWarImporter, AttackersAreExtracted)
{
   WarImporter importer;
   std::stringstream input;
   input << R"(
{
	initiator=144
	target=169
	initiators={
		149 154 159 164
	}
	war=196
}
)";
   std::optional<War> war = importer.ImportWar(input);
   EXPECT_TRUE(war.has_value());
   EXPECT_THAT(war.value_or(War({})).GetAttackers(), testing::ElementsAre(144, 149, 154, 159, 164));
}


TEST(Vic3WorldWarImporter, DefendersAreExtracted)
{
   WarImporter importer;
   std::stringstream input;
   input << R"(
{
	initiator=144
	target=169
	war=196
	targets={
		170 171 172
	}
}
)";
   std::optional<War> war = importer.ImportWar(input);
   EXPECT_TRUE(war.has_value());
   EXPECT_THAT(war.value_or(War({})).GetDefenders(), testing::ElementsAre(169, 170, 171, 172));
}

}  // namespace vic3
