#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

#include "src/mappers/ideology/ideology_mapper.h"
#include "src/mappers/ideology/ideology_mapper_importer.h"



namespace mappers
{

TEST(MappersIdeologyIdeologyMapperImporterTests, RulesCanBeImported)
{
   const IdeologyMapper ideology_mapper = ImportIdeologyMapper("test_files/mappers/ideology/ideology_mappings.txt");

   EXPECT_EQ(ideology_mapper.GetRulingIdeology({"law_council_republic"}), "communism");
   EXPECT_EQ(ideology_mapper.GetSubIdeology("communism", {"law_council_republic"}), "stalinism");
}

}  // namespace mappers