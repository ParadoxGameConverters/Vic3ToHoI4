#include <sstream>

#include "external/googletest/googletest/include/gtest/gtest.h"
#include "src/hoi4_world/map/hoi4_provinces.h"



TEST(Hoi4worldMapHoi4ProvincesTests, MissingMapDefinitionsThrowsException)
{
   commonItems::ModFilesystem mod_filesystem("", {});
   EXPECT_THROW(std::ignore = hoi4::ImportProvinces(mod_filesystem), std::runtime_error);
}


TEST(Hoi4worldMapHoi4ProvincesTests, MapDefinitionsCanBeImported)
{
   commonItems::ModFilesystem mod_filesystem("test_files/hoi4_world", {});
   const auto provinces = hoi4::ImportProvinces(mod_filesystem);

   const std::map<int, hoi4::Province> expected_provinces{{1, hoi4::Province{false, "lakes"}},
       {2, hoi4::Province{true, "forest"}}};
   EXPECT_EQ(provinces, expected_provinces);
}