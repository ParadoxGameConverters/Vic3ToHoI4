#include <sstream>

#include "external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/googletest/googletest/include/gtest/gtest.h"
#include "src/vic3_world/states/vic3_state_importer.h"



namespace vic3
{

TEST(Vic3WorldStateVic3StateImporter, DefaultsAreDefaulted)
{
   std::stringstream input;
   const auto state = StateImporter{}.ImportState(input);

   EXPECT_TRUE(state.GetProvinces().empty());
}


TEST(Vic3WorldStateVic3StateImporter, ExceptionWhenProvinceHasOddNumber)
{
   std::stringstream input;
   input << "={\n";
   input << "\tprovinces={\n";
   input << "\t\tprovinces = { 37330 1 37333 9 37348 }\n";
   input << "\t}";
   input << "}";

   EXPECT_THROW(const auto _ = StateImporter{}.ImportState(input), std::runtime_error);
}


TEST(Vic3WorldStateVic3StateImporter, ItemsCanBeInput)
{
   std::stringstream input;
   input << "={\n";
   input << "\tprovinces={\n";
   input << "\t\tprovinces = { 37330 1 37333 9 37348 1 }\n";
   input << "\t}";
   input << "}";
   const auto state = StateImporter{}.ImportState(input);

   EXPECT_THAT(state.GetProvinces(),
       testing::UnorderedElementsAre(37330,
           37331,
           37333,
           37334,
           37335,
           37336,
           37337,
           37338,
           37339,
           37340,
           37341,
           37342,
           37348,
           37349));
}


TEST(Vic3WorldStateVic3StateImporter, MultipleStatesCanBeInput)
{
   std::stringstream input_one;
   input_one << "={\n";
   input_one << "\tprovinces={\n";
   input_one << "\t\tprovinces = { 37330 1 37333 9 37348 1 }\n";
   input_one << "\t}";
   input_one << "}";
   const auto state_one = StateImporter{}.ImportState(input_one);

   std::stringstream input_two;
   const auto state_two = StateImporter{}.ImportState(input_two);

   EXPECT_THAT(state_one.GetProvinces(),
       testing::UnorderedElementsAre(37330,
           37331,
           37333,
           37334,
           37335,
           37336,
           37337,
           37338,
           37339,
           37340,
           37341,
           37342,
           37348,
           37349));
   EXPECT_TRUE(state_two.GetProvinces().empty());
}

}  // namespace vic3