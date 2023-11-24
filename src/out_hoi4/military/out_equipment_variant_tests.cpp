#include <filesystem>
#include <fstream>
#include <sstream>

#include "external/commonItems/OSCompatibilityLayer.h"
#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "external/fmt/include/fmt/format.h"
#include "src/hoi4_world/military/equipment_variant.h"
#include "src/out_hoi4/military/out_equipment_variant.h"



namespace out
{

TEST(Outhoi4MilitaryEquipmentvariant, VariantCanBeOutput)
{
   hoi4::EquipmentVariant variant("test_variant", "test_type", {}, {}, {{"complex_item", "= {\n\tfoo = bar\n}"}});

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
   hoi4::EquipmentVariant variant("test_variant",
       "",
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
   hoi4::EquipmentVariant variant("test_variant",
       "",
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
