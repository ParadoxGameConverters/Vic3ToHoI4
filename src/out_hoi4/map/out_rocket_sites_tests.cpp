#include <filesystem>
#include <fstream>
#include <sstream>

#include "external/commonItems/OSCompatibilityLayer.h"
#include "external/fmt/include/fmt/format.h"
#include "external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/googletest/googletest/include/gtest/gtest.h"
#include "src/hoi4_world/map/strategic_regions.h"
#include "src/out_hoi4/map/out_rocket_sites.h"



namespace out
{


TEST(Outhoi4MapOutrocketsitesTests, ExceptionForBadPath)
{
   EXPECT_THROW(OutputRocketSites("ExceptionForBadPath", {}), std::runtime_error);
}


TEST(Outhoi4MapOutrocketsitesTests, FolderIsCreated)
{
   commonItems::TryCreateFolder("output");
   commonItems::TryCreateFolder("output/Outhoi4MapOutrocketsitesTestsFolderIsCreated");
   commonItems::TryCreateFolder("output/Outhoi4MapOutrocketsitesTestsFolderIsCreated/map");

   OutputRocketSites("Outhoi4MapOutrocketsitesTestsFolderIsCreated", {});

   EXPECT_TRUE(commonItems::DoesFolderExist("output/Outhoi4MapOutrocketsitesTestsFolderIsCreated/map"));
}


TEST(Outhoi4MapOutrocketsitesTests, RocketSitesAreOutput)
{
   commonItems::TryCreateFolder("output");
   commonItems::TryCreateFolder("output/RocketSitesAreOutput");
   commonItems::TryCreateFolder("output/RocketSitesAreOutput/map");

   OutputRocketSites("RocketSitesAreOutput",
       {hoi4::State(3, std::nullopt, {6, 9}), hoi4::State(4, std::nullopt, {8, 12})});

   ASSERT_TRUE(commonItems::DoesFileExist("output/RocketSitesAreOutput/map/rocketsites.txt"));
   std::ifstream rocket_sites_file("output/RocketSitesAreOutput/map/rocketsites.txt");
   ASSERT_TRUE(rocket_sites_file.is_open());
   std::stringstream rocket_sites_file_stream;
   std::copy(std::istreambuf_iterator<char>(rocket_sites_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(rocket_sites_file_stream));
   rocket_sites_file.close();
   EXPECT_EQ(rocket_sites_file_stream.str(),
       "3={6 }\n"
       "4={8 }\n");
}

}  // namespace out