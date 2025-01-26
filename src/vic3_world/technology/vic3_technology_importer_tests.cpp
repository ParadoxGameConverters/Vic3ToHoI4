#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

#include "src/vic3_world/technology/vic3_technology_importer.h"



namespace vic3
{

TEST(Vic3WorldTechnologyVic3technologyimporterTests, AcquiredTechnologiesDefaultToEmpty)
{
   std::stringstream input;
   const std::map<int, std::set<std::string>> technologies = ImportAcquiredTechnologies(input);

   EXPECT_TRUE(technologies.empty());
}


TEST(Vic3WorldTechnologyVic3technologyimporterTests, AcquiredTechnologiesCanBeImported)
{
   std::stringstream input;
   input << "={\n";
   input << "\tdatabase={\n";
   input << "123456={\n";
   input << "\tcountry=123456\n";
   input << "\tacquired_technologies={ technology technology_two }\n";
   input << "}\n";
   input << "1={\n";
   input << "\tcountry=1\n";
   input << "\tacquired_technologies={ technology_three }\n";
   input << "}\n";
   input << "2={\n";
   input << "\tcountry=2\n";
   input << "}\n";
   input << "\t}\n";
   input << "}";
   const std::map<int, std::set<std::string>> technologies = ImportAcquiredTechnologies(input);

   EXPECT_THAT(technologies,
       testing::UnorderedElementsAre(testing::Pair(1, std::set<std::string>{"technology_three"}),
           testing::Pair(2, std::set<std::string>{}),
           testing::Pair(123456, std::set<std::string>{"technology", "technology_two"})));
}


TEST(Vic3WorldTechnologyVic3technologyimporterTests, NoneIsHandledGracefully)
{
   std::stringstream input;
   input << "={\n";
   input << "\tdatabase={\n";
   input << "123456={\n";
   input << "\tcountry=123456\n";
   input << "\tacquired_technologies={ technology technology_two }\n";
   input << "}\n";
   input << "1=none\n";
   input << "2={\n";
   input << "\tcountry=2\n";
   input << "}\n";
   input << "\t}\n";
   input << "}";
   const std::map<int, std::set<std::string>> technologies = ImportAcquiredTechnologies(input);

   EXPECT_THAT(technologies,
       testing::UnorderedElementsAre(testing::Pair(2, std::set<std::string>{}),
           testing::Pair(123456, std::set<std::string>{"technology", "technology_two"})));
}


TEST(Vic3WorldTechnologyVic3technologyimporterTests, MissingCountrySectionMeansNoEntry)
{
   std::stringstream input;
   input << "={\n";
   input << "\tdatabase={\n";
   input << "1={\n";
   input << "\tacquired_technologies={ technology_three }\n";
   input << "}\n";
   input << "\t}\n";
   input << "}";
   const std::map<int, std::set<std::string>> technologies = ImportAcquiredTechnologies(input);

   EXPECT_TRUE(technologies.empty());
}

}  // namespace vic3