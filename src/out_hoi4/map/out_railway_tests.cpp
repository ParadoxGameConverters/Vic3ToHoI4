#include <filesystem>
#include <fstream>
#include <sstream>

#include "external/commonItems/OSCompatibilityLayer.h"
#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "external/fmt/include/fmt/format.h"
#include "src/hoi4_world/map/railway.h"
#include "src/out_hoi4/map/out_railway.h"



namespace out
{

TEST(Outhoi4MapOutrailwayTests, RailwayIsOutput)
{
   std::stringstream output;
   const hoi4::Railway railway(42, {2, 4, 8});

   output << railway;

   EXPECT_EQ(output.str(), "42 3 2 4 8\n");
}

}  // namespace out