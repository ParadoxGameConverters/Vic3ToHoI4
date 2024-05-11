#include <memory>
#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/hoi4_world/technology/technologies.h"
#include "src/hoi4_world/technology/technologies_converter.h"
#include "src/mappers/technology/tech_mapping.h"



namespace hoi4
{

TEST(Hoi4worldTechnologyTechnologiesconverterTests, NoVic3TechsGivesNoHoi4Techs)
{
   const std::set<std::string> old_technologies;
   const Technologies technologies =
       ConvertTechnologies(old_technologies, {{{"vic3_tech"}, std::nullopt, {"hoi4_tech_one", "hoi4_tech_two"}}});

   EXPECT_TRUE(technologies.GetTechnologies().empty());
}


TEST(Hoi4worldTechnologyTechnologiesconverterTests, NonmatchingVic3TechsGiveNoHoi4Techs)
{
   const std::set<std::string> old_technologies{"non_matching_tech"};
   const Technologies technologies = ConvertTechnologies(old_technologies, {});

   EXPECT_TRUE(technologies.GetTechnologies().empty());
}


TEST(Hoi4worldTechnologyTechnologiesconverterTests, MatchingVic3TechGivesHoi4Techs)
{
   const std::set<std::string> old_technologies{"vic3_tech"};
   const Technologies technologies =
       ConvertTechnologies(old_technologies, {{{"vic3_tech"}, std::nullopt, {"hoi4_tech_one", "hoi4_tech_two"}}});

   EXPECT_THAT(technologies.GetTechnologies(),
       testing::UnorderedElementsAre(
           testing::Pair(std::nullopt, std::set<std::string>{"hoi4_tech_one", "hoi4_tech_two"})));
}


TEST(Hoi4worldTechnologyTechnologiesconverterTests, OnlyOneInstanceOfEachTech)
{
   const std::set<std::string> old_technologies{"vic3_tech", "vic3_tech_two"};
   const Technologies technologies = ConvertTechnologies(old_technologies,
       {
           {{"vic3_tech"}, std::nullopt, {"hoi4_tech_one", "hoi4_tech_two"}},
           {{"vic3_tech_two"}, std::nullopt, {"hoi4_tech_two", "hoi4_tech_three"}},
       });

   EXPECT_THAT(technologies.GetTechnologies(),
       testing::UnorderedElementsAre(
           testing::Pair(std::nullopt, std::set<std::string>{"hoi4_tech_one", "hoi4_tech_two", "hoi4_tech_three"})));
}


TEST(Hoi4worldTechnologyTechnologiesconverterTests, ConvertedTechsAreCategorizedByLimits)
{
   const std::set<std::string> old_technologies{"vic3_non_mtg_naval_tech", "vic3_mtg_naval_tech"};
   const Technologies technologies = ConvertTechnologies(old_technologies,
       {{{"vic3_non_mtg_naval_tech"},
            R"(not = { has_dlc = "Man the Guns" })",
            {"hoi4_non_mtg_naval_tech_one", "hoi4_non_mtg_naval_tech_two"}},
           {{"vic3_mtg_naval_tech"},
               R"(has_dlc = "Man the Guns")",
               {"hoi4_mtg_naval_tech_one", "hoi4_mtg_naval_tech_one"}}});

   EXPECT_THAT(technologies.GetTechnologies(),
       testing::UnorderedElementsAre(
           testing::Pair(std::optional<std::string>{R"(not = { has_dlc = "Man the Guns" })"},
               std::set<std::string>{"hoi4_non_mtg_naval_tech_one", "hoi4_non_mtg_naval_tech_two"}),
           testing::Pair(std::optional<std::string>{R"(has_dlc = "Man the Guns")"},
               std::set<std::string>{"hoi4_mtg_naval_tech_one", "hoi4_mtg_naval_tech_one"})));
}


TEST(Hoi4worldTechnologyTechnologiesconverterTests, NoTechIfNotAllRequirementsAreMet)
{
   const std::set<std::string> old_technologies{"requirement1"};
   const Technologies technologies = ConvertTechnologies(old_technologies,
       {{{"requirement1", "requirement2"}, std::nullopt, {"test_tech1", "test_tech2"}}});

   EXPECT_TRUE(technologies.GetTechnologies().empty());
}


TEST(Hoi4worldTechnologyTechnologiesconverterTests, MultipleVic3RequirementsCanSucceed)
{
   const std::set<std::string> old_technologies{"requirement1", "requirement2"};
   const Technologies technologies = ConvertTechnologies(old_technologies,
       {{{"requirement1", "requirement2"}, std::nullopt, {"test_tech1", "test_tech2"}}});

   EXPECT_THAT(technologies.GetTechnologies(),
       testing::UnorderedElementsAre(testing::Pair(std::nullopt, std::set<std::string>{"test_tech1", "test_tech2"})));
}

}  // namespace hoi4