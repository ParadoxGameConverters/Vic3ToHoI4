#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/vic3_world/institutions/institutions_importer.h"


namespace vic3
{

TEST(Vic3WorldInstitutionsInstitutionsImporter, NoInstitutionsOnEmptyInput)
{
   std::stringstream input;
   std::map<int, std::vector<Institution>> institutions;

   InstitutionsImporter importer(institutions);

   importer(input);

   EXPECT_TRUE(institutions.empty());
}

TEST(Vic3WorldInstitutionsInstitutionsImporter, InstitutionsCanBeImported)
{
   std::stringstream input;
   input << "={\n";
   input << "\tdatabase={\n";
   input << "0 = {\n";
   input << "\tinstitution=institution_schools	investment=3\n";
   input << "\tcountry=38\n";
   input << "}\n";
   input << "1 ={ \n";
   input << "\tinstitution=institution_police	investment=1\n";
   input << "\tcountry=38\n";
   input << "}\n";
   input << "2 = {\n";
   input << "\tinstitution=institution_colonial_affairs	investment=4\n";
   input << "\tcountry=197\n";
   input << "}\n";
   input << "\t}\n";
   input << "}\n";

   std::map<int, std::vector<Institution>> institutions;

   InstitutionsImporter importer(institutions);

   importer(input);

   EXPECT_THAT(institutions,
       testing::UnorderedElementsAre(
           testing::Pair(38, std::vector<Institution>{{"institution_schools", 38, 3}, {"institution_police", 38, 1}}),
           testing::Pair(197, std::vector<Institution>{{"institution_colonial_affairs", 197, 4}})));
}

}  // namespace vic3