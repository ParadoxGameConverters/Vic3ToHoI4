#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

#include "src/vic3_world/elections/elections_importer.h"

namespace vic3
{


TEST(Vic3WorldElectionsElectionsImporter, NoElectionsOnEmptyInput)
{
   std::stringstream input;
   const std::map<int, date> elections = ImportElections(input);

   EXPECT_TRUE(elections.empty());
}


TEST(Vic3WorldElectionsElectionsImporter, ElectionsCanBeImported)
{
   std::stringstream input;
   input << "={\n";
   input << "\tdatabase={\n";
   input << "0 = {\n";
   input << "\tcountry = 1\n";
   input << "\tlast_election = 1839.9.29\n";
   input << "}\n";
   input << "1 ={ \n";
   input << "\tcountry = 3019898882\n";
   input << "\tlast_election = 1935.11.4\n";
   input << "}\n";
   input << "\t}\n";
   input << "}\n";

   const std::map<int, date> elections = ImportElections(input);

   EXPECT_THAT(elections,
       testing::UnorderedElementsAre(testing::Pair(1, date{"1839.9.29"}),
           testing::Pair(-1275068414, date{"1935.11.4"})));
}


TEST(Vic3WorldElectionsElectionsImporter, ElectionsSetAsNoneAreSkipped)
{
   std::stringstream input;
   input << "={\n";
   input << "\tdatabase={\n";
   input << "0 = none\n";
   input << "\t}\n";
   input << "}\n";

   const std::map<int, date> elections = ImportElections(input);

   EXPECT_TRUE(elections.empty());
}



TEST(Vic3WorldElectionsElectionsImporter, ElectionsWithoutCountryAreLogged)
{
   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   std::stringstream input;
   input << "={\n";
   input << "\tdatabase={\n";
   input << "0 = {\n";
   input << "\tlast_election = 1839.9.29\n";
   input << "}\n";
   input << "\t}\n";
   input << "}\n";

   const std::map<int, date> _ = ImportElections(input);

   std::cout.rdbuf(cout_buffer);

   EXPECT_THAT(log.str(),
       testing::HasSubstr(
           R"([WARNING] Election without a country. Please report to the converters team and upload your save.)"));
}

TEST(Vic3WorldElectionsElectionsImporter, ElectionsImportsAreLogged)
{
   std::stringstream input;
   input << "={\n";
   input << "\tdatabase={\n";
   input << "0 = {\n";
   input << "\tcountry = 2\n";
   input << "\tlast_election = 1935.11.4\n";
   input << "}\n";
   input << "3 = none\n";
   input << "4 = none\n";
   input << "5 = none\n";
   input << "\t}\n";
   input << "}\n";

   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const std::map<int, date> _ = ImportElections(input);

   std::cout.rdbuf(cout_buffer);

   EXPECT_THAT(log.str(), testing::HasSubstr("[INFO] \tImported 1 democracies."));
   EXPECT_THAT(log.str(), testing::HasSubstr("[INFO] \tImported 3 democratic backslides."));
}


}  // namespace vic3