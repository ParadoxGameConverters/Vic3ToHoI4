#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

#include "src/mappers/character/leader_type_mapper.h"
#include "src/mappers/character/leader_type_mapper_importer.h"



namespace mappers
{

TEST(MappersCharacterLeadertypemapper, DefaultToHeadOfStateWhenNoGovernancePrinciple)  // NOLINT(cert-err58-cpp)
{
   const LeaderTypeMapper leader_type_mapper =
       ImportLeaderTypeMapper("test_files/configurables/leader_type_mappings.txt");

   EXPECT_EQ(leader_type_mapper.GetCountryLeaderType({}), "head_of_state");
}


TEST(MappersCharacterLeadertypemapper, DefaultToHeadOfStateWhenNoPowerLaw)  // NOLINT(cert-err58-cpp)
{
   const LeaderTypeMapper leader_type_mapper =
       ImportLeaderTypeMapper("test_files/configurables/leader_type_mappings.txt");

   EXPECT_EQ(leader_type_mapper.GetCountryLeaderType({"law_monarchy"}), "head_of_state");
}


TEST(MappersCharacterLeadertypemapper, PrimeMinisterMaps)  // NOLINT(cert-err58-cpp)
{
   const LeaderTypeMapper leader_type_mapper =
       ImportLeaderTypeMapper("test_files/configurables/leader_type_mappings.txt");

   EXPECT_EQ(leader_type_mapper.GetCountryLeaderType({"law_monarchy", "law_wealth_voting"}), "prime_minister");
   EXPECT_EQ(leader_type_mapper.GetCountryLeaderType({"law_monarchy", "law_census_voting"}), "prime_minister");
}


TEST(MappersCharacterLeadertypemapper, CouncilMaps)  // NOLINT(cert-err58-cpp)
{
   const LeaderTypeMapper leader_type_mapper =
       ImportLeaderTypeMapper("test_files/configurables/leader_type_mappings.txt");

   EXPECT_EQ(leader_type_mapper.GetCountryLeaderType({"law_monarchy", "law_anarchy"}), "council");
}


TEST(MappersCharacterLeadertypemapper, LeaderTypesSpecificToGovernanceLaw)  // NOLINT(cert-err58-cpp)
{
   const LeaderTypeMapper leader_type_mapper =
       ImportLeaderTypeMapper("test_files/configurables/leader_type_mappings.txt");

   EXPECT_EQ(leader_type_mapper.GetCountryLeaderType({"law_theocracy", "law_wealth_voting"}), "head_of_state");
}

}  // namespace mappers