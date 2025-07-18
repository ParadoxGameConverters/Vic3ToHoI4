#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <optional>
#include <sstream>

#include "src/vic3_world/military/combat_units_importer.h"



namespace vic3
{

TEST(Vic3worldMilitaryCombatUnitsImporter, EmptyInputMeansNoFormations)
{
   std::stringstream input;
   input << "={\n";
   input << "\tdatabase={\n";
   input << "\t}\n";
   input << "}\n";
   const std::vector<CombatUnit> combat_units = ImportCombatUnits(input);

   EXPECT_TRUE(combat_units.empty());
}


TEST(Vic3worldMilitaryCombatUnitsImporter, DeadIsSkipped)
{
   std::stringstream input;
   input << "={\n";
   input << "\tdatabase={\n";
   input << "\t}\n";
   input << "\tdead={\n";
   input << "\t}\n";
   input << "}\n";

   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const std::vector<CombatUnit> combat_units = ImportCombatUnits(input);

   std::cout.rdbuf(cout_buffer);

   EXPECT_TRUE(log.str().empty());
}


TEST(Vic3worldMilitaryCombatUnitsImporter, UnitsCanBeInput)
{
   std::stringstream input;
   input << "={\n";
   input << "\tdatabase={\n";
   input << "\t\t50331648 = {\n";
   input << "\t\t\tcurrent_manpower = 1000\n";
   input << "\t\t\ttype = \"combat_unit_type_torpedo_boat\"\n";
   input << "\t\t\tcountry = 1\n";
   input << "\t\t\tculture = 294\n";
   input << "\t\t\tbuilding = 6628\n";
   input << "\t\t\tformation = 142\n";
   input << "\t\t\tname_number = 1\n";
   input << "\t\t\tname = \"Bhupesh\"\n";
   input << "\t\t\txp_towards_next_level = 0.65\n";
   input << "\t\t\tcurrent_veterancy_level = 4\n";
   input << "\t\t}\n";
   input << "\t\t83886081 = {\n";
   input << "\t\t\tcurrent_manpower = 0\n";
   input << "\t\t\ttype = \"combat_unit_type_trench_infantry\"\n";
   input << "\t\t\tcountry = 5\n";
   input << "\t\t\tculture = 22\n";
   input << "\t\t\tbuilding = 6058\n";
   input << "\t\t\tformation = 121\n";
   input << "\t\t\tname_number = 584\n";
   input << "\t\t\txp_towards_next_level = 1.05\n";
   input << "\t\t\tcurrent_veterancy_level = 0\n";
   input << "\t\t}\n";
   input << "\t}\n";
   input << "}\n";
   const std::vector<CombatUnit> combat_units = ImportCombatUnits(input);

   EXPECT_THAT(combat_units,
       testing::ElementsAre(
           CombatUnit{
               .current_manpower = 1000,
               .type = "combat_unit_type_torpedo_boat",
               .country = 1,
               .formation = 142,
           },
           CombatUnit{
               .current_manpower = 0,
               .type = "combat_unit_type_trench_infantry",
               .country = 5,
               .formation = 121,
           }));
}

}  // namespace vic3