#include <sstream>

#include "external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/googletest/googletest/include/gtest/gtest.h"
#include "src/hoi4_world/countries/country.h"
#include "src/hoi4_world/world/world.h"
#include "src/mappers/country/country_mapper.h"
#include "src/vic3_world/countries/country.h"
#include "src/vic3_world/world/vic3_world.h"



namespace hoi4
{

TEST(Hoi4worldWorldWorld, EmptyWorldIsEmpty)
{
   constexpr mappers::CountryMapper country_mapper;
   const vic3::World source_world({}, {});

   const World world(source_world, country_mapper);

   EXPECT_TRUE(world.GetCountries().empty());
}


TEST(Hoi4worldWorldWorld, CountriesAreConverted)
{
   constexpr mappers::CountryMapper country_mapper;
   const vic3::Country source_country("TAG");
   const vic3::Country source_country_two("TWO");

   const vic3::World source_world({{1, source_country}, {3, source_country_two}}, {});

   const World world(source_world, country_mapper);

   EXPECT_THAT(world.GetCountries(),
       testing::ElementsAre(testing::Pair("TAG", Country("TAG")), testing::Pair("TWO", Country("TWO"))));
}

}  // namespace hoi4