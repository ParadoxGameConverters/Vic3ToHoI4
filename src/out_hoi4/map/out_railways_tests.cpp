#include <filesystem>
#include <fstream>
#include <sstream>

#include "external/commonItems/OSCompatibilityLayer.h"
#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "external/fmt/include/fmt/format.h"
#include "src/hoi4_world/map/railways.h"
#include "src/out_hoi4/map/out_railways.h"



namespace out
{


TEST(Outhoi4MapOutrailwaysTests, ExceptionForBadPath)
{
   EXPECT_THROW(OutputRailways("ExceptionForBadPath", {}), std::runtime_error);
}


TEST(Outhoi4MapOutrailwaysTests, FilesAreCreated)
{
   commonItems::DeleteFolder("output/Outhoi4MapOutrailwaysTestsFilesAreCreated");
   commonItems::TryCreateFolder("output");
   commonItems::TryCreateFolder("output/Outhoi4MapOutrailwaysTestsFilesAreCreated");
   commonItems::TryCreateFolder("output/Outhoi4MapOutrailwaysTestsFilesAreCreated/map");

   OutputRailways("Outhoi4MapOutrailwaysTestsFilesAreCreated", {});

   EXPECT_TRUE(commonItems::DoesFileExist("output/Outhoi4MapOutrailwaysTestsFilesAreCreated/map/railways.txt"));
}


TEST(Outhoi4MapOutrailwaysTests, RailwaysAreOutput)
{
   commonItems::DeleteFolder("output/Outhoi4MapBuildingsTestsRailwaysAreOutput");
   commonItems::TryCreateFolder("output");
   commonItems::TryCreateFolder("output/Outhoi4MapBuildingsTestsRailwaysAreOutput");
   commonItems::TryCreateFolder("output/Outhoi4MapBuildingsTestsRailwaysAreOutput/map");

   OutputRailways("Outhoi4MapBuildingsTestsRailwaysAreOutput",
       {
           hoi4::Railway{1, {1, 2}},
           hoi4::Railway{2, {2, 4, 8}},
           hoi4::Railway{0, {3, 9, 27}},
       });

   ASSERT_TRUE(commonItems::DoesFileExist("output/Outhoi4MapBuildingsTestsRailwaysAreOutput/map/railways.txt"));
   std::ifstream buildings_file("output/Outhoi4MapBuildingsTestsRailwaysAreOutput/map/railways.txt");
   ASSERT_TRUE(buildings_file.is_open());
   std::stringstream buildings_file_stream;
   std::copy(std::istreambuf_iterator<char>(buildings_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(buildings_file_stream));
   buildings_file.close();
   EXPECT_EQ(buildings_file_stream.str(),
       "1 2 1 2\n"
       "2 3 2 4 8\n");
}

}  // namespace out