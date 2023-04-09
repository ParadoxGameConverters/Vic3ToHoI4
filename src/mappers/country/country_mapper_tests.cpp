#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/mappers/country/country_mapper.h"



namespace mappers
{

TEST(MappersCountryCountryMapper, NulloptIsReturnedForUnmappedTag)
{
   const CountryMapper country_mapper;
   EXPECT_EQ(country_mapper.GetHoiTag(1), std::nullopt);
}


TEST(MappersCountryCountryMapper, MappedTagsAreReturned)
{
   const CountryMapper country_mapper(std::map<int, std::string>{{1, "HOI"}});
   EXPECT_EQ(country_mapper.GetHoiTag(1), "HOI");
}

}  // namespace mappers