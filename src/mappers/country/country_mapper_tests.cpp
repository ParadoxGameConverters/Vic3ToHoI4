#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/mappers/country/country_mapper.h"



namespace mappers
{

TEST(MappersCountryCountryMapper, NulloptIsReturnedForUnmappedTag)
{
   const CountryMapper country_mapper;
   EXPECT_EQ(country_mapper.GetHoiTag("TAG"), std::nullopt);
}


TEST(MappersCountryCountryMapper, MappedTagsAreReturned)
{
   const CountryMapper country_mapper(std::map<std::string, std::string>{{"VIC", "HOI"}});
   EXPECT_EQ(country_mapper.GetHoiTag("VIC"), "HOI");
}

}  // namespace mappers