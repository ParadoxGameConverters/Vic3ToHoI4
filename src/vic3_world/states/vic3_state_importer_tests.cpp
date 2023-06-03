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
   EXPECT_FALSE(state.IsIncorporated());
   EXPECT_TRUE(state.GetProvinces().empty());
   EXPECT_EQ(state.GetPopulation(), 0);
   EXPECT_EQ(state.GetEmployedPopulation(), 0);
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
   input << "\tcountry=42\n";
   input << "\tincorporation = 1\n";
   input << "\tprovinces={\n";
   input << "\t\tprovinces = { 37330 1 37333 9 37348 1 }\n";
   input << "\t}";
   input << "\tpop_statistics={\n";
   input << "\t\tpopulation_lower_strata=2\n";
   input << "\t\tpopulation_middle_strata=4\n";
   input << "\t\tpopulation_upper_strata=6\n";
   input << "\t\tpopulation_salaried_workforce=8\n";
   input << "\t\tpopulation_subsisting_workforce=10\n";  // subsisting workers are not counted for employed workers
   input << "\t\tpopulation_government_workforce=12\n";
   input << "\t\tpopulation_military_workforce=14\n";
   input << "\t\tpopulation_laborer_workforce=16\n";
   input << "\t\tpopulation_unemployed_workforce=18\n";
   input << "\t}\n";
   input << "}";
   const auto state = StateImporter{}.ImportState(input);

   EXPECT_EQ(state.GetOwnerNumber(), 42);
   EXPECT_FALSE(state.GetOwnerTag().has_value());
   EXPECT_TRUE(state.IsIncorporated());
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
   EXPECT_EQ(state.GetPopulation(), 12);
   EXPECT_EQ(state.GetEmployedPopulation(), 68);
}


TEST(Vic3worldStateVic3stateimporter, Pre1_3PopStatisticsCanBeImported)
{
   std::stringstream input;
   input << "={\n";
   input << "\tpop_statistics={\n";
   input << "\t\tlower_strata_pops=2\n";
   input << "\t\tmiddle_strata_pops=4\n";
   input << "\t\tupper_strata_pops=6\n";
   input << "\t\tsalaried_working_adults=8\n";
   input << "\t\tunemployed_working_adults=10\n";
   input << "\t\tlaborer_working_adults=12\n";
   input << "\t\tsubsisting_working_adults=14\n";  // subsisting workers are not counted for employed workers
   input << "\t}\n";
   input << "}";
   const auto state = StateImporter{}.ImportState(input);

   EXPECT_EQ(state.GetPopulation(), 12);
   EXPECT_EQ(state.GetEmployedPopulation(), 18);
}


TEST(Vic3worldStateVic3stateimporter, MultipleStatesCanBeInput)
{
   StateImporter state_importer;

   std::stringstream input_one;
   input_one << "={\n";
   input_one << "\tcountry=42\n";
   input_one << "\tprovinces={\n";
   input_one << "\t\tprovinces = { 37330 1 37333 9 37348 1 }\n";
   input_one << "\t}";
   input_one << "\tpop_statistics={\n";
   input_one << "\t\tlower_strata_pops=2";
   input_one << "\t\tmiddle_strata_pops=4";
   input_one << "\t\tupper_strata_pops=6";
   input_one << "\t\tsalaried_working_adults=8";
   input_one << "\t\tunemployed_working_adults=10";
   input_one << "\t\tlaborer_working_adults=12";
   input_one << "\t\tsubsisting_working_adults=14";  // subsisting workers are not counted for employed workers
   input_one << "\t}\n";
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
   EXPECT_EQ(state_one.GetPopulation(), 12);
   EXPECT_EQ(state_one.GetEmployedPopulation(), 18);

   EXPECT_FALSE(state_two.GetOwnerNumber().has_value());
   EXPECT_FALSE(state_two.GetOwnerTag().has_value());
   EXPECT_TRUE(state_two.GetProvinces().empty());
   EXPECT_EQ(state_two.GetPopulation(), 0);
   EXPECT_EQ(state_two.GetEmployedPopulation(), 0);
}

}  // namespace vic3