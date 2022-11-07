#include <sstream>

#include "external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/googletest/googletest/include/gtest/gtest.h"
#include "src/vic3_world/states/vic3_states_importer.h"



namespace vic3
{

TEST(Vic3WorldStateVic3StatesImporter, NoStatesByDefault)
{
   std::stringstream input;
   const auto states = StatesImporter(false).ImportStates(input);

   EXPECT_TRUE(states.empty());
}


TEST(Vic3WorldStateVic3StatesImporter, StatesCanBeImported)
{
   std::stringstream input;
   input << "={\n";
   input << "\tdatabase = {\n";
   input << "0 = {\n";
   input << "\tprovinces={\n";
   input << "\t\tprovinces={ 1 2 }\n";
   input << "\t}\n";
   input << "}\n";
   input << "1 = {\n";
   input << "\tprovinces={\n";
   input << "\t\tprovinces={ 10 2 }\n";
   input << "\t}\n";
   input << "}\n";
   const auto states = StatesImporter(false).ImportStates(input);

   EXPECT_THAT(states,
       testing::UnorderedElementsAre(testing::Pair(0, State({1, 2, 3})), testing::Pair(1, State({10, 11, 12}))));
}

}  // namespace vic3