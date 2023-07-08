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
   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

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

   std::cout.rdbuf(cout_buffer);

   EXPECT_THAT(log.str(),
       testing::HasSubstr("Missing ideology mapping rule or sub-ideology mapping rule for unmatched_law"));
}


TEST(MappersIdeologyIdeologyMapperTests, UnmatchedIdeologyMeansDespotismSubIdeology)
{
   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const IdeologyMapper ideology_mapper({}, {});
   EXPECT_EQ(ideology_mapper.GetSubIdeology("unmatched_ideology", {}), "despotism");

   std::cout.rdbuf(cout_buffer);

   EXPECT_THAT(log.str(), testing::HasSubstr("No sub-ideology mapping rules for unmatched_ideology"));
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
   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

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

   std::cout.rdbuf(cout_buffer);

   EXPECT_THAT(log.str(),
       testing::HasSubstr("Missing ideology mapping rule or sub-ideology mapping rule for unmatched_law"));
}

}  // namespace mappers