#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

#include "src/vic3_world/pacts/pacts_importer.h"

namespace vic3
{


TEST(Vic3WorldPactsPactsImporter, NoPactsOnEmptyInput)
{
   std::stringstream input;
   const std::map<int, Pact> pacts = ImportPacts(input);

   EXPECT_TRUE(pacts.empty());
}


TEST(Vic3WorldPactsPactsImporter, PactsCanBeImported)
{
   std::stringstream input;
   input << R""(
	database={
0={
		first=1
		second=3
		action=puppet start_date=1836.1.1
}
1={
		first=3
		second=1
		action=alliance start_date=1836.1.1
}
	}
)"";

   const std::map<int, Pact> pacts = ImportPacts(input);

   EXPECT_THAT(pacts,
       testing::UnorderedElementsAre(testing::Pair(0, Pact(1, 3, "puppet", date("1836.1.1"), std::nullopt)),
           testing::Pair(1, Pact(3, 1, "alliance", date("1836.1.1"), std::nullopt))));
}


TEST(Vic3WorldPactsPactsImporter, PactsSetAsNoneAreSkipped)
{
   std::stringstream input;
   input << "={\n";
   input << "\tdatabase={\n";
   input << "0 = none\n";
   input << "\t}\n";
   input << "}\n";

   const std::map<int, Pact> pacts = ImportPacts(input);

   EXPECT_TRUE(pacts.empty());
}


TEST(Vic3WorldPactsPactsImporter, PactsImportsAreLogged)
{
   std::stringstream input;
   input << R""(
={
	database={
0={
		first=1
		second=3
		action=puppet start_date=1836.1.1
}
1={
		first=3
		second=1
		action=alliance start_date=1836.1.1
}
	}
}
)"";
   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const std::map<int, Pact> _ = ImportPacts(input);

   std::cout.rdbuf(cout_buffer);

   EXPECT_THAT(log.str(), testing::HasSubstr("[INFO] \tImported 2 diplomatic pacts.\n"));
}
}  // namespace vic3