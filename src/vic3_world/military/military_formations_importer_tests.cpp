#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <optional>
#include <sstream>

#include "src/vic3_world/military/military_formations_importer.h"



namespace vic3
{

TEST(Vic3worldMilitaryMilitaryFormationsImporter, EmptyInputMeansNoFormations)
{
   std::stringstream input;
   input << "={\n";
   input << "\tdatabase={\n";
   input << "\t}\n";
   input << "}\n";
   const std::map<int, MilitaryFormation> military_formations = ImportMilitaryFormations(input);

   EXPECT_TRUE(military_formations.empty());
}


TEST(Vic3worldMilitaryMilitaryFormationsImporter, ExtrasAreSkipped)
{
   std::stringstream input;
   input << "={\n";
   input << "\tdatabase={\n";
   input << "\t}\n";
   input << "\tdead={\n";
   input << "\t}\n";
   input << "\tfront_to_distribution_data_map={\n";
   input << "\t}\n";
   input << "\tfront_to_distribution_counter_map={\n";
   input << "\t}\n";
   input << "}\n";

   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   [[maybe_unused]] const std::map<int, MilitaryFormation> military_formations = ImportMilitaryFormations(input);

   std::cout.rdbuf(cout_buffer);

   EXPECT_TRUE(log.str().empty());
}


TEST(Vic3worldMilitaryMilitaryFormationsImporter, FormationsCanBeInput)
{
   std::stringstream input;
   input << "={\n";
   input << "\tdatabase={\n";
   input << "1234={\n";
   input << "  country=3019898882\n";
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
   input << "5678={\n";
   input << "  country=12345\n";
   input << "  type=fleet\n";
   input << "}\n";
   input << "\t}\n";
   input << "}\n";
   const std::map<int, MilitaryFormation> military_formations = ImportMilitaryFormations(input);

   EXPECT_THAT(military_formations,
       testing::UnorderedElementsAre(testing::Pair(1234,
                                         MilitaryFormation{
                                             .country = -1275068414,
                                             .type = MilitaryFormationType::kArmy,
                                             .name = "Formation Name",
                                             .ordinal_number = 2,
                                             .units =
                                                 {
                                                     {"combat_unit_type_shrapnel_artillery", 2},
                                                     {"combat_unit_type_dragoons", 3},
                                                     {"combat_unit_type_trench_infantry", 24},
                                                     {"combat_unit_type_siege_artillery", 2},
                                                     {"combat_unit_type_skirmish_infantry", 8},
                                                 },
                                         }),
           testing::Pair(5678,
               MilitaryFormation{
                   .country = 12345,
                   .type = MilitaryFormationType::kFleet,
               })));
}

}  // namespace vic3