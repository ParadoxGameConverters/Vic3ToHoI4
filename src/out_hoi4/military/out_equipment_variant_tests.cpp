#include <external/commonItems/OSCompatibilityLayer.h>
#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>
#include <external/fmt/include/fmt/format.h>

#include <filesystem>
#include <fstream>
#include <sstream>

#include "src/hoi4_world/military/equipment_variant.h"
#include "src/out_hoi4/military/out_equipment_variant.h"



namespace out
{

TEST(Outhoi4MilitaryEquipmentvariant, VariantCanBeOutput)
{
   const hoi4::EquipmentVariant variant(hoi4::EquipmentVariantName{"test_variant"},
       hoi4::EquipmentVariantType{"test_type"},
       {},
       {},
       {{"complex_item", "= {\n\tfoo = bar\n}"}});

   std::stringstream out;
   out << variant;

   EXPECT_EQ(out.str(),
       "\tcreate_equipment_variant = {\n"
       "\t\tname = \"test_variant\"\n"
       "\t\ttype = test_type\n"
       "\t\tcomplex_item = {\n\tfoo = bar\n}\n"
       "\t}\n");
}


TEST(Outhoi4MilitaryEquipmentvariant, RequiredTechsAreNotOutput)
{
   const hoi4::EquipmentVariant variant(hoi4::EquipmentVariantName{"test_variant"},
       hoi4::EquipmentVariantType{""},
       {"required_tech_one", "required_tech_two"},
       {},
       {{"complex_item", "= {\n\tfoo = bar\n}"}});

   std::stringstream out;
   out << variant;

   EXPECT_EQ(out.str(),
       "\tcreate_equipment_variant = {\n"
       "\t\tname = \"test_variant\"\n"
       "\t\tcomplex_item = {\n\tfoo = bar\n}\n"
       "\t}\n");
}


TEST(Outhoi4MilitaryEquipmentvariant, BlockingTechsAreNotOutput)
{
   const hoi4::EquipmentVariant variant(hoi4::EquipmentVariantName{"test_variant"},
       hoi4::EquipmentVariantType{""},
       {},
       {"blocking_tech_one", "blocking_tech_two"},
       {{"complex_item", "= {\n\tfoo = bar\n}"}});

   std::stringstream out;
   out << variant;

   EXPECT_EQ(out.str(),
       "\tcreate_equipment_variant = {\n"
       "\t\tname = \"test_variant\"\n"
       "\t\tcomplex_item = {\n\tfoo = bar\n}\n"
       "\t}\n");
}

}  // namespace out
