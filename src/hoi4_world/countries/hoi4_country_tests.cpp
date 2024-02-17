#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/hoi4_world/countries/hoi4_country.h"



TEST(Hoi4worldCountriesCountry, PuppetsCanBeRemoved)
{
   hoi4::Country country({.puppets = {"ONE", "TWO", "THR", "FOR"}});

   country.RemovePuppets({"TWO", "FOR"});

   EXPECT_THAT(country.GetPuppets(), testing::UnorderedElementsAre("ONE", "THR"));
}