#include <external/commonItems/OSCompatibilityLayer.h>
#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>
#include <external/fmt/include/fmt/format.h>

#include <filesystem>
#include <fstream>
#include <sstream>

#include "src/hoi4_world/map/strategic_region.h"
#include "src/out_hoi4/map/out_strategic_region.h"



namespace out
{


TEST(Outhoi4MapStrategicregionTests, ExceptionWhenFileNotOpened)
{
   const hoi4::StrategicRegion strategic_region_one({.id = 1});

   EXPECT_THROW(OutputStrategicRegion("ExceptionWhenFileNotOpened", strategic_region_one), std::runtime_error);
}


TEST(Outhoi4MapStrategicregionTests, FileIsFromFilename)
{
   commonItems::TryCreateFolder("output");
   commonItems::TryCreateFolder("output/FileIsFromFilename");
   commonItems::TryCreateFolder("output/FileIsFromFilename/map");
   commonItems::TryCreateFolder("output/FileIsFromFilename/map/strategicregions");

   const hoi4::StrategicRegion strategic_region_one({.filename = "strategic_region_one.txt", .id = 1});
   const hoi4::StrategicRegion strategic_region_two({.filename = "strategic_region_two.txt", .id = 2});

   OutputStrategicRegion("FileIsFromFilename", strategic_region_one);
   OutputStrategicRegion("FileIsFromFilename", strategic_region_two);

   EXPECT_TRUE(commonItems::DoesFileExist("output/FileIsFromFilename/map/strategicregions/strategic_region_one.txt"));
   EXPECT_TRUE(commonItems::DoesFileExist("output/FileIsFromFilename/map/strategicregions/strategic_region_two.txt"));
}


TEST(Outhoi4MapStrategicregionTests, BasicsAreOutput)
{
   commonItems::TryCreateFolder("output");
   commonItems::TryCreateFolder("output/BasicsAreOutput");
   commonItems::TryCreateFolder("output/BasicsAreOutput/map");
   commonItems::TryCreateFolder("output/BasicsAreOutput/map/strategicregions");

   const hoi4::StrategicRegion strategic_region_one({.filename = "strategic_region_one.txt", .id = 1});

   OutputStrategicRegion("BasicsAreOutput", strategic_region_one);

   ASSERT_TRUE(commonItems::DoesFileExist("output/BasicsAreOutput/map/strategicregions/strategic_region_one.txt"));
   std::ifstream strategic_region_file("output/BasicsAreOutput/map/strategicregions/strategic_region_one.txt");
   ASSERT_TRUE(strategic_region_file.is_open());
   std::stringstream strategic_region_file_stream;
   std::copy(std::istreambuf_iterator<char>(strategic_region_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(strategic_region_file_stream));
   strategic_region_file.close();
   EXPECT_EQ(strategic_region_file_stream.str(),
       "\n"
       "strategic_region={\n"
       "\tid=1\n"
       "\tname=\"\"\n"
       "\tprovinces={\n"
       "\t\t\n"
       "\t}\n"
       "\tweather\n"
       "}");
}


TEST(Outhoi4MapStrategicregionTests, IdIsOutput)
{
   commonItems::TryCreateFolder("output");
   commonItems::TryCreateFolder("output/IdIsOutput");
   commonItems::TryCreateFolder("output/IdIsOutput/map");
   commonItems::TryCreateFolder("output/IdIsOutput/map/strategicregions");

   const hoi4::StrategicRegion strategic_region_one({.filename = "strategic_region_one.txt", .id = 42});

   OutputStrategicRegion("IdIsOutput", strategic_region_one);

   ASSERT_TRUE(commonItems::DoesFileExist("output/IdIsOutput/map/strategicregions/strategic_region_one.txt"));
   std::ifstream strategic_region_file("output/IdIsOutput/map/strategicregions/strategic_region_one.txt");
   ASSERT_TRUE(strategic_region_file.is_open());
   std::stringstream strategic_region_file_stream;
   std::copy(std::istreambuf_iterator<char>(strategic_region_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(strategic_region_file_stream));
   strategic_region_file.close();
   EXPECT_EQ(strategic_region_file_stream.str(),
       "\n"
       "strategic_region={\n"
       "\tid=42\n"
       "\tname=\"\"\n"
       "\tprovinces={\n"
       "\t\t\n"
       "\t}\n"
       "\tweather\n"
       "}");
}


TEST(Outhoi4MapStrategicregionTests, NameIsOutput)
{
   commonItems::TryCreateFolder("output");
   commonItems::TryCreateFolder("output/NameIsOutput");
   commonItems::TryCreateFolder("output/NameIsOutput/map");
   commonItems::TryCreateFolder("output/NameIsOutput/map/strategicregions");

   const hoi4::StrategicRegion strategic_region_one(
       {.filename = "strategic_region_one.txt", .id = 1, .name = "strategic_region_name"});

   OutputStrategicRegion("NameIsOutput", strategic_region_one);

   ASSERT_TRUE(commonItems::DoesFileExist("output/NameIsOutput/map/strategicregions/strategic_region_one.txt"));
   std::ifstream strategic_region_file("output/NameIsOutput/map/strategicregions/strategic_region_one.txt");
   ASSERT_TRUE(strategic_region_file.is_open());
   std::stringstream strategic_region_file_stream;
   std::copy(std::istreambuf_iterator<char>(strategic_region_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(strategic_region_file_stream));
   strategic_region_file.close();
   EXPECT_EQ(strategic_region_file_stream.str(),
       "\n"
       "strategic_region={\n"
       "\tid=1\n"
       "\tname=\"strategic_region_name\"\n"
       "\tprovinces={\n"
       "\t\t\n"
       "\t}\n"
       "\tweather\n"
       "}");
}


TEST(Outhoi4MapStrategicregionTests, OldProvincesAreNotOutput)
{
   commonItems::TryCreateFolder("output");
   commonItems::TryCreateFolder("output/OldProvincesAreNotOutput");
   commonItems::TryCreateFolder("output/OldProvincesAreNotOutput/map");
   commonItems::TryCreateFolder("output/OldProvincesAreNotOutput/map/strategicregions");

   const hoi4::StrategicRegion strategic_region_one(
       {.filename = "strategic_region_one.txt", .id = 1, .old_provinces = {42, 144}});

   OutputStrategicRegion("OldProvincesAreNotOutput", strategic_region_one);

   ASSERT_TRUE(
       commonItems::DoesFileExist("output/OldProvincesAreNotOutput/map/strategicregions/strategic_region_one.txt"));
   std::ifstream strategic_region_file("output/OldProvincesAreNotOutput/map/strategicregions/strategic_region_one.txt");
   ASSERT_TRUE(strategic_region_file.is_open());
   std::stringstream strategic_region_file_stream;
   std::copy(std::istreambuf_iterator<char>(strategic_region_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(strategic_region_file_stream));
   strategic_region_file.close();
   EXPECT_EQ(strategic_region_file_stream.str(),
       "\n"
       "strategic_region={\n"
       "\tid=1\n"
       "\tname=\"\"\n"
       "\tprovinces={\n"
       "\t\t\n"
       "\t}\n"
       "\tweather\n"
       "}");
}


TEST(Outhoi4MapStrategicregionTests, NewProvincesAreOutput)
{
   commonItems::TryCreateFolder("output");
   commonItems::TryCreateFolder("output/NewProvincesAreOutput");
   commonItems::TryCreateFolder("output/NewProvincesAreOutput/map");
   commonItems::TryCreateFolder("output/NewProvincesAreOutput/map/strategicregions");

   hoi4::StrategicRegion strategic_region_one({.filename = "strategic_region_one.txt", .id = 1});
   strategic_region_one.AddNewProvince(144);
   strategic_region_one.AddNewProvince(169);

   OutputStrategicRegion("NewProvincesAreOutput", strategic_region_one);

   ASSERT_TRUE(
       commonItems::DoesFileExist("output/NewProvincesAreOutput/map/strategicregions/strategic_region_one.txt"));
   std::ifstream strategic_region_file("output/NewProvincesAreOutput/map/strategicregions/strategic_region_one.txt");
   ASSERT_TRUE(strategic_region_file.is_open());
   std::stringstream strategic_region_file_stream;
   std::copy(std::istreambuf_iterator<char>(strategic_region_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(strategic_region_file_stream));
   strategic_region_file.close();
   EXPECT_EQ(strategic_region_file_stream.str(),
       "\n"
       "strategic_region={\n"
       "\tid=1\n"
       "\tname=\"\"\n"
       "\tprovinces={\n"
       "\t\t144 169 \n"
       "\t}\n"
       "\tweather\n"
       "}");
}


TEST(Outhoi4MapStrategicregionTests, StaticModifiersAreOutput)
{
   commonItems::TryCreateFolder("output");
   commonItems::TryCreateFolder("output/StaticModifiersAreOutput");
   commonItems::TryCreateFolder("output/StaticModifiersAreOutput/map");
   commonItems::TryCreateFolder("output/StaticModifiersAreOutput/map/strategicregions");

   const hoi4::StrategicRegion strategic_region_one({.filename = "strategic_region_one.txt",
       .id = 1,
       .static_modifiers{{"static_modifer_one", "always"}, {"static_modifier_two", "never"}}});

   OutputStrategicRegion("StaticModifiersAreOutput", strategic_region_one);

   ASSERT_TRUE(
       commonItems::DoesFileExist("output/StaticModifiersAreOutput/map/strategicregions/strategic_region_one.txt"));
   std::ifstream strategic_region_file("output/StaticModifiersAreOutput/map/strategicregions/strategic_region_one.txt");
   ASSERT_TRUE(strategic_region_file.is_open());
   std::stringstream strategic_region_file_stream;
   std::copy(std::istreambuf_iterator<char>(strategic_region_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(strategic_region_file_stream));
   strategic_region_file.close();
   EXPECT_EQ(strategic_region_file_stream.str(),
       "\n"
       "strategic_region={\n"
       "\tid=1\n"
       "\tname=\"\"\n"
       "\tprovinces={\n"
       "\t\t\n"
       "\t}\n"
       "\tstatic_modifiers={\n"
       "\t\tstatic_modifer_one=always\n"
       "\t\tstatic_modifier_two=never\n"
       "\n"
       "\t}\n"
       "\tweather\n"
       "}");
}


TEST(Outhoi4MapStrategicregionTests, NavalTerrainIsOutput)
{
   commonItems::TryCreateFolder("output");
   commonItems::TryCreateFolder("output/NavalTerrainIsOutput");
   commonItems::TryCreateFolder("output/NavalTerrainIsOutput/map");
   commonItems::TryCreateFolder("output/NavalTerrainIsOutput/map/strategicregions");

   const hoi4::StrategicRegion strategic_region_one(
       {.filename = "strategic_region_one.txt", .id = 1, .naval_terrain = "test_naval_terrain"});

   OutputStrategicRegion("NavalTerrainIsOutput", strategic_region_one);

   ASSERT_TRUE(commonItems::DoesFileExist("output/NavalTerrainIsOutput/map/strategicregions/strategic_region_one.txt"));
   std::ifstream strategic_region_file("output/NavalTerrainIsOutput/map/strategicregions/strategic_region_one.txt");
   ASSERT_TRUE(strategic_region_file.is_open());
   std::stringstream strategic_region_file_stream;
   std::copy(std::istreambuf_iterator<char>(strategic_region_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(strategic_region_file_stream));
   strategic_region_file.close();
   EXPECT_EQ(strategic_region_file_stream.str(),
       "\n"
       "strategic_region={\n"
       "\tid=1\n"
       "\tname=\"\"\n"
       "\tprovinces={\n"
       "\t\t\n"
       "\t}\n"
       "\tnaval_terrain=test_naval_terrain\n"
       "\tweather\n"
       "}");
}


TEST(Outhoi4MapStrategicregionTests, WeatherIsOutput)
{
   commonItems::TryCreateFolder("output");
   commonItems::TryCreateFolder("output/WeatherIsOutput");
   commonItems::TryCreateFolder("output/WeatherIsOutput/map");
   commonItems::TryCreateFolder("output/WeatherIsOutput/map/strategicregions");

   const hoi4::StrategicRegion strategic_region_one({.filename = "strategic_region_one.txt",
       .id = 1,
       .weather = "={\n"
                  "\t\tfoo={\n"
                  "\t\t\tbar=baz\n"
                  "\t\t}\n"
                  "\t}"});

   OutputStrategicRegion("WeatherIsOutput", strategic_region_one);

   ASSERT_TRUE(commonItems::DoesFileExist("output/WeatherIsOutput/map/strategicregions/strategic_region_one.txt"));
   std::ifstream strategic_region_file("output/WeatherIsOutput/map/strategicregions/strategic_region_one.txt");
   ASSERT_TRUE(strategic_region_file.is_open());
   std::stringstream strategic_region_file_stream;
   std::copy(std::istreambuf_iterator<char>(strategic_region_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(strategic_region_file_stream));
   strategic_region_file.close();
   EXPECT_EQ(strategic_region_file_stream.str(),
       "\n"
       "strategic_region={\n"
       "\tid=1\n"
       "\tname=\"\"\n"
       "\tprovinces={\n"
       "\t\t\n"
       "\t}\n"
       "\tweather={\n"
       "\t\tfoo={\n"
       "\t\t\tbar=baz\n"
       "\t\t}\n"
       "\t}\n"
       "}");
}


}  // namespace out