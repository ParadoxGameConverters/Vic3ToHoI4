#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/mappers/ideology/ideology_mapper.h"



namespace mappers
{

TEST(MappersIdeologyIdeologyMapperTests, NoRulesMeansNeutralityIdeology)
{
   const IdeologyMapper ideology_mapper({}, {});
   EXPECT_EQ(ideology_mapper.GetRulingIdeology({}), "neutrality");
}


TEST(MappersIdeologyIdeologyMapperTests, HighestRatedIdeologyWins)
{
   const IdeologyMapper ideology_mapper(
       {
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
       },
       {});

   EXPECT_EQ(ideology_mapper.GetRulingIdeology({"law_parliamentary_republic", "law_landed_voting"}), "democratic");
}


TEST(MappersIdeologyIdeologyMapperTests, NoMatchedLawsMeansNeutralityIdeology)
{
   const IdeologyMapper ideology_mapper(
       {
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
       },
       {});

   EXPECT_EQ(ideology_mapper.GetRulingIdeology({"unmatched_law"}), "neutrality");
}


TEST(MappersIdeologyIdeologyMapperTests, UnmatchedIdeologyMeansDespotismSubIdeology)
{
   const IdeologyMapper ideology_mapper({}, {});
   EXPECT_EQ(ideology_mapper.GetSubIdeology("unmatched_ideology", {}), "despotism");
}


TEST(MappersIdeologyIdeologyMapperTests, HighestRatedSubIdeologyWins)
{
   const IdeologyMapper ideology_mapper({},
       {
           {
               "matched_ideology",
               {
                   {"law_parliamentary_republic",
                       {
                           {"sub_ideology_one", 50},
                           {"sub_ideology_two", 50},
                           {"sub_ideology_three", 25},
                           {"sub_ideology_four", 25},
                       }},
                   {"law_landed_voting",
                       {
                           {"sub_ideology_one", 25},
                           {"sub_ideology_two", -50},
                           {"sub_ideology_three", 0},
                           {"sub_ideology_four", 0},
                       }},
               },
           },
           {
               "unmatched_ideology",
               {
                   {"law_parliamentary_republic",
                       {
                           {"sub_ideology_five", 500},
                       }},
                   {"law_landed_voting",
                       {
                           {"sub_ideology_five", 250},
                       }},
               },
           },
       });

   EXPECT_EQ(ideology_mapper.GetSubIdeology("matched_ideology", {"law_parliamentary_republic", "law_landed_voting"}),
       "sub_ideology_one");
}


TEST(MappersIdeologyIdeologyMapperTests, NoMatchedLawsMeansDespotismSubIdeology)
{
   const IdeologyMapper ideology_mapper({},
       {
           {
               "matched_ideology",
               {
                   {"law_parliamentary_republic",
                       {
                           {"sub_ideology_one", 50},
                           {"sub_ideology_two", 50},
                           {"sub_ideology_three", 25},
                           {"sub_ideology_four", 25},
                       }},
                   {"law_landed_voting",
                       {
                           {"sub_ideology_one", 25},
                           {"sub_ideology_two", -50},
                           {"sub_ideology_three", 0},
                           {"sub_ideology_four", 0},
                       }},
               },
           },
           {
               "unmatched_ideology",
               {
                   {"law_parliamentary_republic",
                       {
                           {"sub_ideology_five", 500},
                       }},
                   {"law_landed_voting",
                       {
                           {"sub_ideology_five", 250},
                       }},
               },
           },
       });

   EXPECT_EQ(ideology_mapper.GetSubIdeology("matched_ideology", {"unmatched_law"}), "despotism");
}

}  // namespace mappers