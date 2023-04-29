#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/mappers/ideology/ideology_mapper.h"



namespace mappers
{

// These tests

TEST(MappersIdeologyIdeologyMapperCreator, DefaultRulingIdeologyIsNeutrality)
{
   constexpr IdeologyMapper ideology_mapper;
   EXPECT_EQ(ideology_mapper.GetRulingIdeology({}), "neutrality");
}


TEST(MappersIdeologyIdeologyMapperCreator, DemocracyForAppropriateLaws)
{
   constexpr IdeologyMapper ideology_mapper;
   EXPECT_EQ(ideology_mapper.GetRulingIdeology({"law_landed_voting"}), "democratic");
}

}  // namespace mappers