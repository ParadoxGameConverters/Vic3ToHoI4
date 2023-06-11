#include <filesystem>
#include <sstream>

#include "external/commonItems/OSCompatibilityLayer.h"
#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"



namespace out
{

TEST(Outhoi4NameTests, NameIsOutput)
{
   std::stringstream output;
   const std::string a;

   output << a;

   EXPECT_EQ(output.str(), "42;test_type;4.25;9.25;16.25;25.25;144\n");
}

}  // namespace out