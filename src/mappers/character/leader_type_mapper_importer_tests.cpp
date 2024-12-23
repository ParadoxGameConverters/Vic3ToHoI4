#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

#include "src/mappers/character/leader_type_mapper_importer.h"



namespace mappers
{

TEST(MappersCharacterLeadertypemapperimporter, MappingsCanBeImported)
{
   const LeaderTypeMapper leader_type_mapper =
       ImportLeaderTypeMapper("test_files/configurables/leader_type_mappings.txt");

   EXPECT_EQ(leader_type_mapper.GetCountryLeaderType({"law_monarchy", "law_wealth_voting"}), "prime_minister");
   EXPECT_EQ(leader_type_mapper.GetCountryLeaderType({"law_monarchy", "law_census_voting"}), "prime_minister");
   EXPECT_EQ(leader_type_mapper.GetCountryLeaderType({"law_monarchy", "law_anarchy"}), "council");
}


TEST(MappersCharacterLeadertypemapperimporter, UnexpectedLeaderTypesAreLogged)
{
   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const LeaderTypeMapper _ = ImportLeaderTypeMapper("test_files/configurables/leader_type_mappings.txt");

   std::cout.rdbuf(cout_buffer);

   EXPECT_THAT(log.str(), testing::HasSubstr("[DEBUG]     Ignoring keyword: typo_councl"));
}

}  // namespace mappers