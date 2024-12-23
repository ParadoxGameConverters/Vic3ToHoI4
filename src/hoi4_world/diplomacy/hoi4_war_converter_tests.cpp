#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include <sstream>

#include "hoi4_war_converter.h"
#include "src/hoi4_world/diplomacy/hoi4_war.h"
#include "src/mappers/country/country_mapper.h"
#include "src/vic3_world/wars/war.h"



namespace hoi4
{


TEST(Hoi4WorldDiplomacyWarTests, OriginalDefenderCanBeConverted)
{
   const std::optional<War> war = ConvertWar(vic3::War({.original_attacker = 1, .original_defender = 12}),
       std::set<std::string>{},
       mappers::CountryMapper({{1, "ATK"}, {12, "DEF"}}));

   EXPECT_TRUE(war.has_value());
   EXPECT_EQ(war.value_or(War({})).original_defender, "DEF");
}


TEST(Hoi4WorldDiplomacyWarTests, WarnIfOriginalDefenderCantBeMapped)
{
   std::stringstream log;
   auto std_out_buf = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const std::optional<War> war = ConvertWar(vic3::War({.original_attacker = 1, .original_defender = 12}),
       std::set<std::string>{},
       mappers::CountryMapper(std::map<int, std::string>{{1, "ATK"}}));

   std::cout.rdbuf(std_out_buf);

   EXPECT_FALSE(war.has_value());
   EXPECT_THAT(log.str(), testing::HasSubstr("[WARNING] Could not map 12, original defender in a war"));
}


TEST(Hoi4WorldDiplomacyWarTests, ExtraDefendersCanBeAdded)
{
   const std::optional<War> war =
       ConvertWar(vic3::War({.original_attacker = 1, .original_defender = 12, .defenders = {12, 13, 14, 15}}),
           std::set<std::string>{"DF2", "DF3", "DF4"},
           mappers::CountryMapper({{1, "ATK"}, {12, "DEF"}, {13, "DF2"}, {14, "DF3"}, {15, "DF4"}}));

   EXPECT_TRUE(war.has_value());
   EXPECT_THAT(war.value_or(War({})).extra_defenders, testing::ElementsAre("DF2", "DF3", "DF4"));
}


TEST(Hoi4WorldDiplomacyWarTests, WarnIfExtraDefendersCantBeMapped)
{
   std::stringstream log;
   auto std_out_buf = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const std::optional<War> war =
       ConvertWar(vic3::War({.original_attacker = 1, .original_defender = 12, .defenders = {12, 13}}),
           std::set<std::string>{},
           mappers::CountryMapper(std::map<int, std::string>{{1, "ATK"}, {12, "DEF"}}));

   std::cout.rdbuf(std_out_buf);

   EXPECT_TRUE(war.has_value());
   EXPECT_THAT(log.str(), testing::HasSubstr("[WARNING] Could not map 13, defending in a war"));
}


TEST(Hoi4WorldDiplomacyWarTests, OriginalAttackerCanBeConverted)
{
   const std::optional<War> war = ConvertWar(vic3::War({.original_attacker = 12, .original_defender = 1}),
       std::set<std::string>{},
       mappers::CountryMapper({{12, "OAT"}, {1, "DEF"}}));

   EXPECT_TRUE(war.has_value());
   EXPECT_EQ(war.value_or(War({})).original_attacker, "OAT");
}


TEST(Hoi4WorldDiplomacyWarTests, WarnIfOriginalAttackerCantBeMapped)
{
   std::stringstream log;
   auto std_out_buf = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const std::optional<War> war = ConvertWar(vic3::War({.original_attacker = 12, .original_defender = 1}),
       std::set<std::string>{},
       mappers::CountryMapper(std::map<int, std::string>{{1, "DEF"}}));

   std::cout.rdbuf(std_out_buf);

   EXPECT_FALSE(war.has_value());
   EXPECT_THAT(log.str(), testing::HasSubstr("[WARNING] Could not map 12, original attacker in a war"));
}


TEST(Hoi4WorldDiplomacyWarTests, ExtraAttackersCanBeAdded)
{
   const std::optional<War> war =
       ConvertWar(vic3::War({.original_attacker = 12, .attackers = {12, 13, 14, 15}, .original_defender = 1}),
           std::set<std::string>{"AT2", "AT3", "AT4"},
           mappers::CountryMapper({{12, "OAT"}, {13, "AT2"}, {14, "AT3"}, {15, "AT4"}, {1, "DEF"}}));

   EXPECT_TRUE(war.has_value());
   EXPECT_THAT(war.value_or(War({})).extra_attackers, testing::ElementsAre("AT2", "AT3", "AT4"));
}


TEST(Hoi4WorldDiplomacyWarTests, WarnIfExtraAttackersCantBeMapped)
{
   std::stringstream log;
   auto std_out_buf = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const std::optional<War> war =
       ConvertWar(vic3::War({.original_attacker = 12, .attackers = {12, 13}, .original_defender = 1}),
           std::set<std::string>{},
           mappers::CountryMapper(std::map<int, std::string>{{1, "DEF"}, {12, "OAT"}}));

   std::cout.rdbuf(std_out_buf);

   EXPECT_TRUE(war.has_value());
   EXPECT_THAT(log.str(), testing::HasSubstr("[WARNING] Could not map 13, attacking in a war"));
}

}  // namespace hoi4