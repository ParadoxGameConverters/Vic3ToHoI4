#include <sstream>

#include "external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/googletest/googletest/include/gtest/gtest.h"
#include "src/hoi4_world/countries/hoi4_country.h"
#include "src/hoi4_world/world/hoi4_world.h"
#include "src/mappers/country/country_mapper.h"
#include "src/mappers/provinces/province_mapper.h"
#include "src/vic3_world/countries/vic3_country.h"
#include "src/vic3_world/provinces/vic3_province_definitions.h"
#include "src/vic3_world/world/vic3_world.h"



namespace hoi4
{

TEST(Hoi4worldWorldWorld, EmptyWorldIsEmpty)
{
   constexpr mappers::CountryMapper country_mapper;
   const vic3::World source_world({}, {}, vic3::ProvinceDefinitions({}));

   mappers::ProvinceMapper province_mapper{{}, {}};

   const World world(source_world, country_mapper, province_mapper);

   EXPECT_TRUE(world.GetCountries().empty());
   EXPECT_TRUE(world.GetStates().empty());
}


TEST(Hoi4worldWorldWorld, CountriesAreConverted)
{
   constexpr mappers::CountryMapper country_mapper;
   const vic3::Country source_country("TAG");
   const vic3::Country source_country_two("TWO");

   const vic3::World source_world({{1, source_country}, {3, source_country_two}}, {}, vic3::ProvinceDefinitions({}));

   mappers::ProvinceMapper province_mapper{{}, {}};

   const World world(source_world, country_mapper, province_mapper);

   EXPECT_THAT(world.GetCountries(),
       testing::ElementsAre(testing::Pair("TAG", Country("TAG")), testing::Pair("TWO", Country("TWO"))));
}


TEST(Hoi4worldWorldWorld, StatesAreConverted)
{
   constexpr mappers::CountryMapper country_mapper;

   const vic3::World source_world({},
       {{1, vic3::State({1, 2, 3})}, {2, vic3::State({4, 5, 6})}},
       vic3::ProvinceDefinitions({"0x000001", "0x000002", "0x000003", "0x000004", "0x000005", "0x000006"}));

   mappers::ProvinceMapper province_mapper{{},
       {
           {10, {"0x000001"}},
           {20, {"0x000002"}},
           {30, {"0x000003"}},
           {40, {"0x000004"}},
           {50, {"0x000005"}},
           {60, {"0x000006"}},
       }};

   const World world(source_world, country_mapper, province_mapper);

   EXPECT_THAT(world.GetStates(), testing::ElementsAre(State(1, {10, 20, 30}), State(2, {40, 50, 60})));
}

}  // namespace hoi4