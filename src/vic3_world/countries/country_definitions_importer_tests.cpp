#include <external/commonItems/ModLoader/ModFilesystem.h>
#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

#include "src/vic3_world/countries/country_definitions_importer.h"



using std::filesystem::path;



namespace vic3
{

TEST(Vic3WorldCountriesCountriesDefinitionsImporter, NoDefinitionsByInput)
{
   const commonItems::ModFilesystem mod_filesystem("test_files/vic3_world/countries/no_definitions_by_default/game",
       {});
   const auto tags_to_colors = ImportCountryColorDefinitions(mod_filesystem);

   EXPECT_TRUE(tags_to_colors.empty());
}

TEST(Vic3WorldCountriesCountriesDefinitionsImporter, DefinitionsCanBeImported)
{
   const commonItems::ModFilesystem mod_filesystem("test_files/vic3_world/countries/definitions_can_be_imported/game",
       {});
   const auto tags_to_colors = ImportCountryColorDefinitions(mod_filesystem);

   EXPECT_THAT(tags_to_colors,
       testing::UnorderedElementsAre(testing::Pair("ONE", commonItems::Color(std::array{1, 1, 1})),
           testing::Pair("TWO", commonItems::Color(std::array{2, 4, 8})),
           testing::Pair("THR", commonItems::Color(std::array{3, 9, 27}))));
}

}  // namespace vic3