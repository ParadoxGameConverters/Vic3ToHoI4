#include <sstream>

#include "external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/googletest/googletest/include/gtest/gtest.h"
#include "src/mappers/country/country_mapper.h"



namespace vic3
{

TEST(MappersCountryCountryMapper, SameTagIsReturned)
{
   mappers::CountryMapper country_mapper;
   EXPECT_EQ(country_mapper.GetHoiTag("TAG"), "TAG");
   EXPECT_EQ(country_mapper.GetHoiTag("TWO"), "TWO");
}

}  // namespace vic3