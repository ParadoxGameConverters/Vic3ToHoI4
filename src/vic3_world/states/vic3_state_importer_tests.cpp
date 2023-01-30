#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/vic3_world/states/vic3_state_importer.h"



namespace vic3
{

TEST(Vic3worldStateVic3stateimporter, DefaultsAreDefaulted)
{
   std::stringstream input;
   const auto state = StateImporter{}.ImportState(input);

   EXPECT_FALSE(state.GetOwnerNumber().has_value());
   EXPECT_FALSE(state.GetOwnerTag().has_value());
   EXPECT_TRUE(state.GetProvinces().empty());
}


TEST(Vic3worldStateVic3stateimporter, ExceptionWhenProvinceHasOddNumberOfItems)
{
   std::stringstream input;
   input << "={\n";
   input << "\tprovinces={\n";
   input << "\t\tprovinces = { 37330 1 37333 9 37348 }\n";
   input << "\t}";
   input << "}";

   EXPECT_THROW(const auto _ = StateImporter{}.ImportState(input), std::runtime_error);
}


TEST(Vic3worldStateVic3stateimporter, ItemsCanBeInput)
{
   std::stringstream input;
   input << "={\n";
   input << "country=42\n";
   input << "\tprovinces={\n";
   input << "\t\tprovinces = { 37330 1 37333 9 37348 1 }\n";
   input << "\t}";
   input << "}";
   const auto state = StateImporter{}.ImportState(input);

   EXPECT_EQ(state.GetOwnerNumber(), 42);
   EXPECT_FALSE(state.GetOwnerTag().has_value());
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


TEST(Vic3worldStateVic3stateimporter, MultipleStatesCanBeInput)
{
   StateImporter state_importer;

   std::stringstream input_one;
   input_one << "={\n";
   input_one << "country=42\n";
   input_one << "\tprovinces={\n";
   input_one << "\t\tprovinces = { 37330 1 37333 9 37348 1 }\n";
   input_one << "\t}";
   input_one << "}";
   const auto state_one = state_importer.ImportState(input_one);

   std::stringstream input_two;
   const auto state_two = state_importer.ImportState(input_two);

   EXPECT_EQ(state_one.GetOwnerNumber(), 42);
   EXPECT_FALSE(state_one.GetOwnerTag().has_value());
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
   EXPECT_FALSE(state_two.GetOwnerNumber().has_value());
   EXPECT_FALSE(state_two.GetOwnerTag().has_value());
   EXPECT_TRUE(state_two.GetProvinces().empty());
}

}  // namespace vic3