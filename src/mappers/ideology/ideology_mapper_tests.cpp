#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/mappers/ideology/ideology_mapper.h"



namespace mappers
{

TEST(MappersIdeologyIdeologyMapperTests, NoRulesMeansNeutralityIdeology)
{
   const IdeologyMapper ideology_mapper({});
   EXPECT_EQ(ideology_mapper.GetRulingIdeology({}), "neutrality");
}


TEST(MappersIdeologyIdeologyMapperTests, HighestRatedIdeologyWins)
{
   const IdeologyMapper ideology_mapper({
       {"law_parliamentary_republic",
           {
               {"democratic", 50},
               {"communism", 50},
               {"fascism", 25},
               {"neutrality", 25},
           }},
       {"law_landed_voting",
           {
               {"democratic", 25},
               {"communism", -50},
               {"fascism", 0},
               {"neutrality", 0},
           }},
   });

   EXPECT_EQ(ideology_mapper.GetRulingIdeology({"law_parliamentary_republic", "law_landed_voting"}), "democratic");
}


TEST(MappersIdeologyIdeologyMapperTests, NoMatchedLawsMeansNeutralityIdeology)
{
   const IdeologyMapper ideology_mapper({
       {"law_parliamentary_republic",
           {
               {"democratic", 50},
               {"communism", 50},
               {"fascism", 25},
               {"neutrality", 25},
           }},
       {"law_landed_voting",
           {
               {"democratic", 25},
               {"communism", -50},
               {"fascism", 0},
               {"neutrality", 0},
           }},
   });

   EXPECT_EQ(ideology_mapper.GetRulingIdeology({"unmatched_law"}), "neutrality");
}

}  // namespace mappers