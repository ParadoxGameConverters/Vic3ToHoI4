#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/vic3_world/wars/wars_importer.h"



namespace vic3
{

TEST(Vic3WorldWarsImporter, WarsCanBeImported)
{
   std::stringstream input;
   input << R"(
{
database={
1={
	initiator=144
	target=169
	war=196
}
1={
	initiator=12
	target=13
	war=31
}
}
}
)";

   const std::vector<War> wars = ImportWars(input);
   EXPECT_THAT(wars,
       testing::ElementsAre(War({.original_attacker = 144, .original_defender = 169}),
           War({.original_attacker = 12, .original_defender = 13})));
}


TEST(Vic3WorldWarsImporter, InvalidWarsAreSkipped)
{
   std::stringstream input;
   input << R"(
{
database={
1={
	initiator=144
	target=169
}
1={
	initiator=12
	target=13
	war=4294967295
}
}
}
)";

   const std::vector<War> wars = ImportWars(input);
   EXPECT_TRUE(wars.empty());
}

}  // namespace vic3