#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

#include "src/hoi4_world/military/equipment_variant.h"
#include "src/hoi4_world/military/equipment_variants_importer.h"



namespace hoi4
{

TEST(Hoi4worldMilitaryEquipmentvariantimporterTests, EquipmentVariantsCanBeImported)
{
   const std::vector<EquipmentVariant> variants =
       ImportEquipmentVariants("test_files/hoi4_world/military/EquipmentVariantsCanBeImported.txt");

   EXPECT_THAT(variants,
       testing::UnorderedElementsAre(
           EquipmentVariant(EquipmentVariantName{"Test Variant One"}, EquipmentVariantType{""}, {}, {}, {}),
           EquipmentVariant(EquipmentVariantName{"Test Variant Two"},
               EquipmentVariantType{""},
               {"required_tech_one", "required_tech_two"},
               {"blocking_tech_one", "blocking_tech_two"},
               {
                   {"item_block",
                       "= {\n"
                       "\t\titem_one = true\n"
                       "\t\titem_two = true\n"
                       "\t}"},
               })));
}

}  // namespace hoi4
