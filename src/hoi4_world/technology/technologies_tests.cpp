#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <memory>
#include <sstream>

#include "src/hoi4_world/technology/technologies.h"



namespace hoi4
{


TEST(Hoi4worldTechnologyTechnologiesTests, HasTechnologyReturnsFalseIfTechNotThere)  // NOLINT(cert-err58-cpp)
{
   const Technologies technologies(std::map<std::optional<std::string>, std::set<std::string>>{});

   EXPECT_FALSE(technologies.HasTechnology("missing_technology"));
}


TEST(Hoi4worldTechnologyTechnologiesTests, HasTechnologyReturnsTrueIfTechIsThere)  // NOLINT(cert-err58-cpp)
{
   const Technologies technologies({{std::nullopt, {"test_technology"}}});

   EXPECT_TRUE(technologies.HasTechnology("test_technology"));
}


TEST(Hoi4worldTechnologyTechnologiesTests, HasTechnologyReturnsTrueIfTechIsThereWithLimit)  // NOLINT(cert-err58-cpp)
{
   const Technologies technologies({{std::nullopt, {"test_technology"}}, {"test_limit", {"test_technology_two"}}});

   EXPECT_TRUE(technologies.HasTechnology("test_technology_two"));
}

}  // namespace hoi4