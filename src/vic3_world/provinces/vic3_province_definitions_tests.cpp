#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/vic3_world/provinces/vic3_province_definitions.h"



namespace vic3
{

TEST(Vic3WorldProvincesVic3ProvinceDefinitions, DefinitionsCanBeReturned)
{
   std::vector<std::string> province_definitions_map{"x000001", "x000200"};

   ProvinceDefinitions province_definitions(province_definitions_map);

   EXPECT_EQ(province_definitions.GetProvinceDefinitions(), province_definitions_map);
}


TEST(Vic3WorldProvincesVic3ProvinceDefinitions, DefinitionCanBeLookedUp)
{
   std::vector<std::string> province_definitions_map{"x000001", "x000200"};

   ProvinceDefinitions province_definitions(province_definitions_map);

   const auto province_one = province_definitions.GetProvinceDefinition(1);
   EXPECT_TRUE(province_one.has_value());
   EXPECT_EQ(*province_one, "x000001");
   const auto province_two = province_definitions.GetProvinceDefinition(2);
   EXPECT_TRUE(province_two.has_value());
   EXPECT_EQ(*province_two, "x000200");
}


TEST(Vic3WorldProvincesVic3ProvinceDefinitions, MissingDefinitionIsNotLookedUp)
{
   std::vector<std::string> province_definitions_map{"x000001", "x000200"};

   ProvinceDefinitions province_definitions(province_definitions_map);

   const auto province_three = province_definitions.GetProvinceDefinition(3);
   EXPECT_FALSE(province_three.has_value());
}

}  // namespace vic3