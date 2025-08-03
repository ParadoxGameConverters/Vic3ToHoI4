#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <optional>
#include <sstream>

#include "src/vic3_world/military/combat_unit.h"
#include "src/vic3_world/military/combat_unit_importer.h"



namespace vic3
{

TEST(Vic3worldMilitaryCombatunitImporter, DefaultsAreDefaulted)
{
   CombatUnitImporter importer;

   std::stringstream input;
   input << "={\n";
   input << "}\n";
   const CombatUnit combat_unit = importer.ImportCombatUnit(input);

   EXPECT_EQ(combat_unit.current_manpower, 0);
   EXPECT_EQ(combat_unit.type, std::nullopt);
   EXPECT_EQ(combat_unit.country, std::nullopt);
   EXPECT_EQ(combat_unit.formation, std::nullopt);
}


TEST(Vic3worldMilitaryCombatunitImporter, ItemsAreInput)
{
   CombatUnitImporter importer;

   std::stringstream input;
   input << "={\n";
   input << "\tcurrent_manpower = 1000\n";
   input << "\ttype = \"combat_unit_type_torpedo_boat\"\n";
   input << "\tcountry = 3019898882\n";
   input << "\tculture = 294\n";
   input << "\tbuilding = 6628\n";
   input << "\tformation = 142\n";
   input << "\tname_number = 1\n";
   input << "\tname = \"Bhupesh\"\n";
   input << "\txp_towards_next_level = 0.65\n";
   input << "\tcurrent_veterancy_level = 4\n";
   input << "}\n";
   const CombatUnit combat_unit = importer.ImportCombatUnit(input);

   EXPECT_EQ(combat_unit.current_manpower, 1000);
   EXPECT_TRUE(combat_unit.type.has_value());
   EXPECT_EQ(combat_unit.type.value_or(""), "combat_unit_type_torpedo_boat");
   EXPECT_TRUE(combat_unit.country.has_value());
   EXPECT_EQ(combat_unit.country.value_or(-1), -1275068414);
   EXPECT_TRUE(combat_unit.formation.has_value());
   EXPECT_EQ(combat_unit.formation.value_or(-1), 142);
}

}  // namespace vic3