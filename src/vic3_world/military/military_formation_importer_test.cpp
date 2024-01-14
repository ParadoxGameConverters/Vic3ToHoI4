#include "src/vic3_world/military/military_formation_importer.h"

#include <optional>
#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"

namespace vic3
{

TEST(Vic3worldMilitaryMilitaryFormationImporter, NoCountryNumberMeanNoImport)
{
   MilitaryFormationImporter importer;

   std::stringstream input;
   const std::optional<MilitaryFormation> military_formation = importer.ImportMilitaryFormation(input);

   EXPECT_FALSE(military_formation.has_value());
}


TEST(Vic3worldMilitaryMilitaryFormationImporter, DefaultsAreDefaulted)
{
   MilitaryFormationImporter importer;

   std::stringstream input;
   input << "={\n";
   input << "  country=12345\n";
   input << "}\n";
   const std::optional<MilitaryFormation> military_formation = importer.ImportMilitaryFormation(input);

   EXPECT_TRUE(military_formation.has_value());
   EXPECT_EQ(military_formation.value_or(MilitaryFormation{}).type, MilitaryFormationType::kArmy);
   EXPECT_EQ(military_formation.value_or(MilitaryFormation{}).country, 12345);
   EXPECT_EQ(military_formation.value_or(MilitaryFormation{}).name, std::nullopt);
   EXPECT_EQ(military_formation.value_or(MilitaryFormation{}).ordinal_number, std::nullopt);
   EXPECT_TRUE(military_formation.value_or(MilitaryFormation{}).units.empty());
}


TEST(Vic3worldMilitaryMilitaryFormationImporter, ItemsCanBeImported)
{
   MilitaryFormationImporter importer;

   std::stringstream input;
   input << "={\n";
   input << "  country=12345\n";
   input << "  type=army\n";
   input << "  name=\"Formation Name\"\n";
   input << "  ordinal_number=2\n";
   // input << "  position={ 4743.02784 910.80912 }\n";  // import this eventually
   // input << "  organization=25\n";                    // import this eventually
   // input << "  current_location={\n";                 // import this eventually
   // input << "    type=hq\n";
   // input << "    identity=50331767\n";
   // input << "  }\n";
   // input << "  target_location={\n";                  // import this eventually
   // input << "    type=hq\n";
   // input << "    identity=50331767\n";
   // input << "  }\n";
   // input << "  travel_progress={\n";                  // import this eventually
   input << "  building_to_expected_units_map={\n";
   input << "8061={\n";
   input << "      unit_types_num_list={\n";
   input << "combat_unit_type_shrapnel_artillery=2 combat_unit_type_dragoons=3 combat_unit_type_trench_infantry=8 "
            "combat_unit_type_siege_artillery=2       }\n";
   input << "    } 11850={\n";
   input << "      unit_types_num_list={\n";
   input << "combat_unit_type_skirmish_infantry=8       }\n";
   input << "    } 50334733={\n";
   input << "      unit_types_num_list={\n";
   input << "combat_unit_type_trench_infantry=16       }\n";
   input << "    }   }\n";
   input << "}\n";
   const std::optional<MilitaryFormation> military_formation = importer.ImportMilitaryFormation(input);

   std::map<std::string, int> expected_units{
       {"combat_unit_type_shrapnel_artillery", 2},
       {"combat_unit_type_dragoons", 3},
       {"combat_unit_type_trench_infantry", 24},
       {"combat_unit_type_siege_artillery", 2},
       {"combat_unit_type_skirmish_infantry", 8},
   };

   EXPECT_TRUE(military_formation.has_value());
   EXPECT_EQ(military_formation.value_or(MilitaryFormation{}).country, 12345);
   EXPECT_EQ(military_formation.value_or(MilitaryFormation{}).type, MilitaryFormationType::kArmy);
   EXPECT_EQ(military_formation.value_or(MilitaryFormation{}).name, std::make_optional("Formation Name"));
   EXPECT_EQ(military_formation.value_or(MilitaryFormation{}).ordinal_number, std::make_optional(2));
   EXPECT_EQ(military_formation.value_or(MilitaryFormation{}).units, expected_units);
}


TEST(Vic3worldMilitaryMilitaryFormationImporter, TypeCanBeFleet)
{
   MilitaryFormationImporter importer;

   std::stringstream input;
   input << "={\n";
   input << "  country=12345\n";
   input << "  type=fleet\n";
   input << "}\n";
   const std::optional<MilitaryFormation> military_formation = importer.ImportMilitaryFormation(input);

   EXPECT_TRUE(military_formation.has_value());
   EXPECT_EQ(military_formation.value_or(MilitaryFormation{}).country, 12345);
   EXPECT_EQ(military_formation.value_or(MilitaryFormation{}).type, MilitaryFormationType::kFleet);
}

}  // namespace vic3
