#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/vic3_world/countries/vic3_country.h"
#include "src/vic3_world/world/vic3_world.h"

TEST(Vic3worldCountriesVic3country, GetAcquiredTechEmpty)
{
   const auto state = vic3::Country(vic3::CountryOptions{
       .number = 2,
   });
   const auto world = vic3::World({.acquired_technologies = {{1, {"tech_a", "tech_b"}}}});
   EXPECT_TRUE(state.GetAcquiredTechnologies(world).empty());
}

TEST(Vic3worldCountriesVic3country, GetAcquiredTech)
{
   const auto state = vic3::Country(vic3::CountryOptions{
       .number = 1,
   });
   const auto world = vic3::World({.acquired_technologies = {{1, {"tech_a", "tech_b"}}}});
   EXPECT_THAT(state.GetAcquiredTechnologies(world), testing::UnorderedElementsAre("tech_a", "tech_b"));
}