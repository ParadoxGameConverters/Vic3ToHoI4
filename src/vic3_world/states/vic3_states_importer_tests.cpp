#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/vic3_world/states/vic3_states_importer.h"



namespace vic3
{

TEST(Vic3worldStateVic3statesimporter, NoStatesByDefault)
{
   std::stringstream input;
   const auto states = ImportStates(input);

   EXPECT_TRUE(states.empty());
}


TEST(Vic3worldStateVic3statesimporter, StatesCanBeImported)
{
   std::stringstream input;
   input << "={\n";
   input << "\tdatabase = {\n";
   input << "0={\n";
   input << "\tcountry=42\n";
   input << "}\n";
   input << "1={\n";
   input << "\tcountry=144\n";
   input << "}\n";
   const auto states = ImportStates(input);

   EXPECT_THAT(states,
       testing::UnorderedElementsAre(testing::Pair(0, State({.owner_number = 42})),
           testing::Pair(1, State({.owner_number = 144}))));
}


TEST(Vic3worldStateVic3statesimporter, StatesIndexesCanBeSkipped)
{
   std::stringstream input;
   input << "={\n";
   input << "\tdatabase = {\n";
   input << "0={\n";
   input << "\tcountry=42\n";
   input << "}\n";
   input << "1=none\n";
   input << "2={\n";
   input << "\tcountry=144\n";
   input << "}\n";
   const auto states = ImportStates(input);

   EXPECT_THAT(states,
       testing::UnorderedElementsAre(testing::Pair(0, State({.owner_number = 42})),
           testing::Pair(2, State({.owner_number = 144}))));
}

}  // namespace vic3