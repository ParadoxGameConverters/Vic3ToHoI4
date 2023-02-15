#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/hoi4_world/technology/technologies.h"
#include "src/hoi4_world/technology/technologies_converter.h"
#include "src/mappers/technology/tech_mapping.h"
// #include "src/Mappers/Technology/ResearchBonusMapper.h"
// #include "src/Mappers/Technology/ResearchBonusMapperFactory.h"
#include <memory>
#include <sstream>



TEST(Hoi4worldTechnologyTechnologiesconverterTests, NoVic3TechsGivesNoHoi4Techs)
{
   const std::set<std::string> old_technologies;
   const hoi4::Technologies technologies = hoi4::ConvertTechnologies(old_technologies,
       {{{"vic3_tech"}, std::nullopt, {"hoi4_tech_one", "hoi4_tech_two"}}} /*,{}*/);

   EXPECT_TRUE(technologies.GetTechnologies().empty());
}


TEST(Hoi4worldTechnologyTechnologiesconverterTests, NonmatchingVic3TechsGiveNoHoi4Techs)
{
   const std::set<std::string> old_technologies{"non_matching_tech"};
   const hoi4::Technologies technologies = hoi4::ConvertTechnologies(old_technologies, {} /*,{}*/);

   EXPECT_TRUE(technologies.GetTechnologies().empty());
}


TEST(Hoi4worldTechnologyTechnologiesconverterTests, MatchingVic3TechGivesHoi4Techs)
{
   const std::set<std::string> old_technologies{"vic3_tech"};
   const hoi4::Technologies technologies = hoi4::ConvertTechnologies(old_technologies,
       {{{"vic3_tech"}, std::nullopt, {"hoi4_tech_one", "hoi4_tech_two"}}} /*,{}*/);

   EXPECT_THAT(technologies.GetTechnologies(),
       testing::UnorderedElementsAre(
           testing::Pair(std::nullopt, std::set<std::string>{"hoi4_tech_one", "hoi4_tech_two"})));
}


TEST(Hoi4worldTechnologyTechnologiesconverterTests, OnlyOneInstanceOfEachTech)
{
   const std::set<std::string> old_technologies{"vic3_tech", "vic3_tech_two"};
   const hoi4::Technologies technologies = hoi4::ConvertTechnologies(old_technologies,
       {
           {{"vic3_tech"}, std::nullopt, {"hoi4_tech_one", "hoi4_tech_two"}},
           {{"vic3_tech_two"}, std::nullopt, {"hoi4_tech_two", "hoi4_tech_three"}},
       } /*,{}*/);

   EXPECT_THAT(technologies.GetTechnologies(),
       testing::UnorderedElementsAre(
           testing::Pair(std::nullopt, std::set<std::string>{"hoi4_tech_one", "hoi4_tech_two", "hoi4_tech_three"})));
}


TEST(Hoi4worldTechnologyTechnologiesconverterTests, ConvertedTechsAreCategorizedByLimits)
{
   const std::set<std::string> old_technologies{"vic3_non_mtg_naval_tech", "vic3_mtg_naval_tech"};
   const hoi4::Technologies technologies = hoi4::ConvertTechnologies(old_technologies,
       {{{"vic3_non_mtg_naval_tech"},
            R"(not = { has_dlc = "Man the Guns" })",
            {"hoi4_non_mtg_naval_tech_one", "hoi4_non_mtg_naval_tech_two"}},
           {{"vic3_mtg_naval_tech"},
               R"(has_dlc = "Man the Guns")",
               {"hoi4_mtg_naval_tech_one", "hoi4_mtg_naval_tech_one"}}} /*,{}*/);

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
   const hoi4::Technologies technologies = hoi4::ConvertTechnologies(old_technologies,
       {{{"requirement1", "requirement2"}, std::nullopt, {"test_tech1", "test_tech2"}}} /*,{}*/);

   EXPECT_TRUE(technologies.GetTechnologies().empty());
}


TEST(Hoi4worldTechnologyTechnologiesconverterTests, MultipleVic3RequirementsCanSucceed)
{
   const std::set<std::string> old_technologies{"requirement1", "requirement2"};
   const hoi4::Technologies technologies = hoi4::ConvertTechnologies(old_technologies,
       {{{"requirement1", "requirement2"}, std::nullopt, {"test_tech1", "test_tech2"}}} /*,{}*/);

   EXPECT_THAT(technologies.GetTechnologies(),
       testing::UnorderedElementsAre(testing::Pair(std::nullopt, std::set<std::string>{"test_tech1", "test_tech2"})));
}


// TEST(Hoi4worldTechnologyTechnologiesconverterTests, noVic3TechsGiveNoResearchBonuses)
//{
//   const std::set<std::string> old_technologies;
//   hoi4::Technologies technologies = hoi4::ConvertTechnologies(old_technologies, {} /*,{}*/);
//
//   std::stringstream outputStream;
//   outputResearchBonuses(technologies, outputStream);
//   std::string output = outputStream.str();
//
//   std::string expectedOutput = "# Research Bonuses\n";
//   ASSERT_EQ(expectedOutput, output);
//}
//
//
// TEST(Hoi4worldTechnologyTechnologiesconverterTests, nonMatchingVic3TechsGiveNoResearchBonuses)
//{
//   std::set<std::string> old_technologies;
//   old_technologies.insert("nonMatchingTech");
//   old_technologies.insert("nonMatchingInvention");
//   hoi4::Technologies technologies = hoi4::ConvertTechnologies(old_technologies, {} /*,{}*/);
//
//   std::stringstream outputStream;
//   outputResearchBonuses(technologies, outputStream);
//   std::string output = outputStream.str();
//
//   std::string expectedOutput = "# Research Bonuses\n";
//   ASSERT_EQ(expectedOutput, output);
//}
//
//
// TEST(Hoi4worldTechnologyTechnologiesconverterTests, matchingVic3TechGivesResearchBonuses)
//{
//   std::set<std::string> old_technologies;
//   old_technologies.insert("Vic3_tech");
//   const hoi4::Technologies technologies = hoi4::ConvertTechnologies(old_technologies, {} /*,{}*/);
//
//   std::stringstream outputStream;
//   outputResearchBonuses(technologies, outputStream);
//
//   std::stringstream expectedOutput;
//   expectedOutput << "# Research Bonuses\n";
//   expectedOutput << "add_doctrine_cost_reduction = {\n";
//   expectedOutput << "\tname = bonus1_doc_bonus\n";
//   expectedOutput << "\tcost_reduction = 5.5\n";
//   expectedOutput << "\tuses = 1\n";
//   expectedOutput << "\tcategory = bonus1_doctrine\n";
//   expectedOutput << "}\n";
//   expectedOutput << "add_doctrine_cost_reduction = {\n";
//   expectedOutput << "\tname = bonus2_doc_bonus\n";
//   expectedOutput << "\tcost_reduction = 6.6\n";
//   expectedOutput << "\tuses = 1\n";
//   expectedOutput << "\tcategory = bonus2_doctrine\n";
//   expectedOutput << "}\n";
//   ASSERT_EQ(expectedOutput.str(), outputStream.str());
//}
//
//
// TEST(Hoi4worldTechnologyTechnologiesconverterTests, matchingVic3InventionGivesResearchBonuses)
//{
//   std::set<std::string> old_technologies;
//   old_technologies.insert("Vic3_invention");
//   const hoi4::Technologies technologies = hoi4::ConvertTechnologies(old_technologies, {} /*,{}*/);
//
//   std::stringstream outputStream;
//   outputResearchBonuses(technologies, outputStream);
//
//   std::stringstream expectedOutput;
//   expectedOutput << "# Research Bonuses\n";
//   expectedOutput << "add_doctrine_cost_reduction = {\n";
//   expectedOutput << "\tname = bonus2_doc_bonus\n";
//   expectedOutput << "\tcost_reduction = 6.6\n";
//   expectedOutput << "\tuses = 1\n";
//   expectedOutput << "\tcategory = bonus2_doctrine\n";
//   expectedOutput << "}\n";
//   expectedOutput << "add_doctrine_cost_reduction = {\n";
//   expectedOutput << "\tname = bonus3_doc_bonus\n";
//   expectedOutput << "\tcost_reduction = 7.7\n";
//   expectedOutput << "\tuses = 1\n";
//   expectedOutput << "\tcategory = bonus3_doctrine\n";
//   expectedOutput << "}\n";
//   EXPECT_EQ(outputStream.str(), expectedOutput.str());
//}
//
//
// TEST(Hoi4worldTechnologyTechnologiesconverterTests, onlyOneInstanceOfEachResearchBonuses)
//{
//   std::set<std::string> old_technologies;
//   old_technologies.insert("Vic3_tech");
//   old_technologies.insert("Vic3_invention");
//   const hoi4::Technologies technologies = hoi4::ConvertTechnologies(old_technologies, {} /*,{}*/);
//
//   std::stringstream outputStream;
//   outputResearchBonuses(technologies, outputStream);
//
//   std::stringstream expectedOutput;
//   expectedOutput << "# Research Bonuses\n";
//   expectedOutput << "add_doctrine_cost_reduction = {\n";
//   expectedOutput << "\tname = bonus1_doc_bonus\n";
//   expectedOutput << "\tcost_reduction = 5.5\n";
//   expectedOutput << "\tuses = 1\n";
//   expectedOutput << "\tcategory = bonus1_doctrine\n";
//   expectedOutput << "}\n";
//   expectedOutput << "add_doctrine_cost_reduction = {\n";
//   expectedOutput << "\tname = bonus2_doc_bonus\n";
//   expectedOutput << "\tcost_reduction = 6.6\n";
//   expectedOutput << "\tuses = 1\n";
//   expectedOutput << "\tcategory = bonus2_doctrine\n";
//   expectedOutput << "}\n";
//   expectedOutput << "add_doctrine_cost_reduction = {\n";
//   expectedOutput << "\tname = bonus3_doc_bonus\n";
//   expectedOutput << "\tcost_reduction = 7.7\n";
//   expectedOutput << "\tuses = 1\n";
//   expectedOutput << "\tcategory = bonus3_doctrine\n";
//   expectedOutput << "}\n";
//   EXPECT_EQ(outputStream.str(), expectedOutput.str());
//}