#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/vic3_world/laws/laws_importer.h"



namespace vic3
{


TEST(Vic3WorldLawsLawsImporter, NoLawsOnEmptyInput)
{
   std::stringstream input;
   const std::map<int, std::set<std::string>> laws = ImportLaws(input);

   EXPECT_TRUE(laws.empty());
}


TEST(Vic3WorldLawsLawsImporter, LawsCanBeImported)
{
   std::stringstream input;
   input << "={\n";
   input << "\tdatabase={\n";
   input << "0 = {\n";
   input << "\tlaw = test_law_one	country = 1\n";
   input << "\tactive = yes\n";
   input << "}\n";
   input << "1 ={ \n";
   input << "\tlaw = test_law_two	country = 1\n";
   input << "\tactive = yes\n";
   input << "}\n";
   input << "2 = {\n";
   input << "\tlaw = test_law_one	country = 2\n";
   input << "\tactive = yes\n";
   input << "}\n";
   input << "3 = {\n";
   input << "\tlaw = test_law_three	country = 2\n";
   input << "\tactive = yes\n";
   input << "}\n";
   input << "\t}\n";
   input << "}\n";

   const std::map<int, std::set<std::string>> laws = ImportLaws(input);

   EXPECT_THAT(laws,
       testing::UnorderedElementsAre(testing::Pair(1, std::set<std::string>{"test_law_one", "test_law_two"}),
           testing::Pair(2, std::set<std::string>{"test_law_one", "test_law_three"})));
}


TEST(Vic3WorldLawsLawsImporter, LawsSetAsNoneAreSkipped)
{
   std::stringstream input;
   input << "={\n";
   input << "\tdatabase={\n";
   input << "0 = none\n";
   input << "\t}\n";
   input << "}\n";

   const std::map<int, std::set<std::string>> laws = ImportLaws(input);

   EXPECT_TRUE(laws.empty());
}


TEST(Vic3WorldLawsLawsImporter, InactiveLawsAreSkipped)
{
   std::stringstream input;
   input << "={\n";
   input << "\tdatabase={\n";
   input << "0 = {\n";
   input << "\tlaw = test_law_one	country = 1\n";
   input << "}\n";
   input << "\t}\n";
   input << "}\n";

   const std::map<int, std::set<std::string>> laws = ImportLaws(input);

   EXPECT_TRUE(laws.empty());
}


TEST(Vic3WorldLawsLawsImporter, LawsWithOddActiveBlockAreLogged)
{
   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   std::stringstream input;
   input << "={\n";
   input << "\tdatabase={\n";
   input << "0 = {\n";
   input << "\tlaw = test_law_one	country = 1\n";
   input << "\tactive=weirdness\n";
   input << "}\n";
   input << "\t}\n";
   input << "}\n";

   const std::map<int, std::set<std::string>> _ = ImportLaws(input);

   std::cout.rdbuf(cout_buffer);

   EXPECT_THAT(log.str(),
       testing::HasSubstr(
           R"([WARNING] Law had odd active block. Please report to the converters team and upload your save.)"));
}


TEST(Vic3WorldLawsLawsImporter, LawsWithoutCountryAreLogged)
{
   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   std::stringstream input;
   input << "={\n";
   input << "\tdatabase={\n";
   input << "0 = {\n";
   input << "\tlaw = test_law_one\n";
   input << "\tactive = yes\n";
   input << "}\n";
   input << "\t}\n";
   input << "}\n";

   const std::map<int, std::set<std::string>> _ = ImportLaws(input);

   std::cout.rdbuf(cout_buffer);

   EXPECT_THAT(log.str(),
       testing::HasSubstr(
           R"([WARNING] Law without a country. Please report to the converters team and upload your save.)"));
}


TEST(Vic3WorldLawsLawsImporter, LawsWithoutNamesAreImported)
{
   std::stringstream input;
   input << "={\n";
   input << "\tdatabase={\n";
   input << "0 = {\n";
   input << "\tcountry = 1\n";
   input << "\tactive = yes\n";
   input << "}\n";
   input << "\t}\n";
   input << "}\n";

   const std::map<int, std::set<std::string>> laws = ImportLaws(input);

   EXPECT_THAT(laws, testing::UnorderedElementsAre(testing::Pair(1, std::set<std::string>{""})));
}


TEST(Vic3WorldLawsLawsImporter, LawsImportsAreLogged)
{
   std::stringstream input;
   input << "={\n";
   input << "\tdatabase={\n";
   input << "0 = {\n";
   input << "\tlaw = test_law_one	country = 1\n";
   input << "\tactive = yes\n";
   input << "}\n";
   input << "1 ={ \n";
   input << "\tlaw = test_law_two	country = 1\n";
   input << "}\n";
   input << "2 = {\n";
   input << "\tlaw = test_law_one	country = 2\n";
   input << "}\n";
   input << "3 = none\n";
   input << "4 = none\n";
   input << "5 = none\n";
   input << "\t}\n";
   input << "}\n";

   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const std::map<int, std::set<std::string>> _ = ImportLaws(input);

   std::cout.rdbuf(cout_buffer);

   EXPECT_THAT(log.str(), testing::HasSubstr("[INFO] \tImported 1 active laws."));
   EXPECT_THAT(log.str(), testing::HasSubstr("[INFO] \tImported 2 inactive laws."));
   EXPECT_THAT(log.str(), testing::HasSubstr("[INFO] \tImported 3 empty laws."));
}


}  // namespace vic3