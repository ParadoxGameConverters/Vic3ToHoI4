#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/vic3_world/cultures/cultures_importer.h"

namespace vic3
{


TEST(Vic3WorldCulturesCulturesImporter, NoCulturesOnEmptyInput)
{
   std::stringstream input;
   const std::map<int, std::string> cultures = ImportCultures(input);

   EXPECT_TRUE(cultures.empty());
}


TEST(Vic3WorldCulturesCulturesImporter, CulturesCanBeImported)
{
   std::stringstream input;
   input << "={\n";
   input << "\tdatabase={\n";
   input << "0 = {\n";
   input << "\ttype = south_german\n";
   input << "}\n";
   input << "1 ={ \n";
   input << "\ttype = north_german\n";
   input << "}\n";
   input << "\t}\n";
   input << "}\n";

   const std::map<int, std::string> cultures = ImportCultures(input);

   EXPECT_THAT(cultures,
       testing::UnorderedElementsAre(testing::Pair(0, "south_german"), testing::Pair(1, "north_german")));
}


TEST(Vic3WorldCulturesCulturesImporter, CulturesSetAsNoneAreMarkedBroken)
{
   std::stringstream input;
   input << "={\n";
   input << "\tdatabase={\n";
   input << "0 = none\n";
   input << "\t}\n";
   input << "}\n";

   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const std::map<int, std::string> cultures = ImportCultures(input);

   EXPECT_THAT(log.str(),
       testing::HasSubstr(R"([ERROR] Broken culture definition in save file. This should not happen.)"));
}


TEST(Vic3WorldCulturesCulturesImporter, CulturesImportsAreLogged)
{
   std::stringstream input;
   input << "={\n";
   input << "\tdatabase={\n";
   input << "0 = {\n";
   input << "\ttype = south_german\n";
   input << "}\n";
   input << "1 ={ \n";
   input << "\ttype = north_german\n";
   input << "}\n";
   input << "\t}\n";
   input << "}\n";

   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const std::map<int, std::string> _ = ImportCultures(input);

   std::cout.rdbuf(cout_buffer);

   EXPECT_THAT(log.str(), testing::HasSubstr("[INFO] \tImported 2 active cultures."));
}


TEST(Vic3WorldCulturesCulturesImporter, NonEmptyDeadCulturesAreLogged)
{
   std::stringstream input;
   input << "={\n";
   input << "\tdead={\n";
   input << "0 = { type = german }\n";
   input << "\t}\n";
   input << "}\n";

   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const std::map<int, std::string> _ = ImportCultures(input);

   std::cout.rdbuf(cout_buffer);

   EXPECT_THAT(log.str(), testing::HasSubstr(R"([INFO] Dead culture definitions found.)"));
}
}  // namespace vic3