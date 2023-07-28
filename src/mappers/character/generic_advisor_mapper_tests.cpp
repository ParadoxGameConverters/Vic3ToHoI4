﻿#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/hoi4_world/characters/hoi4_character.h"
#include "src/mappers/character/generic_advisor_mapper.h"
#include "src/mappers/character/generic_advisor_mapper_importer.h"



namespace mappers
{

TEST(MappersCharacter Genericadvisormapper, NoConvertedAdvisorsReturnsAllGenerics)
{
   const GenericAdvisorMapper generic_advisor_mapper =
       ImportGenericAdvisorMapper("test_files/configurables/generic_advisors.txt");
   EXPECT_THAT(generic_advisor_mapper.SelectGenericAdvisors({}, {}), testing::UnorderedElementsAre(testing::pair("democratic_reformer", "political_advisor"),
   testing::pair("communist_revolutionary", "political_advisor"),
   testing::pair("army_chief_defensive_1", "army_chief"));
}

TEST(MappersCharacter Genericadvisormapper, CollidingConvertedAdvisorsLimitsGenerics)
{
   const GenericAdvisorMapper generic_advisor_mapper =
       ImportGenericAdvisorMapper("test_files/configurables/generic_advisors.txt");
       
    hoi4::Character colliding_advisor
    {
      .id=1,
      .hoi4::Advisor({.slot = "political_advisor", .traits = {"communist_revolutionary"}}),
    }
       
   EXPECT_THAT(generic_advisor_mapper.SelectGenericAdvisors({1}, {{1,colliding_advisor}}), testing::UnorderedElementsAre(testing::pair("democratic_reformer", "political_advisor"),
   testing::pair("army_chief_defensive_1", "army_chief"));
}