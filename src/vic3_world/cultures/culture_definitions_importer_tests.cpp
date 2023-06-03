#include <sstream>

#include "culture_definition.h"
#include "external/commonItems/ModLoader/ModFilesystem.h"
#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/vic3_world/cultures/culture_definitions_importer.h"



namespace vic3
{

TEST(Vic3WorldCountriesCountriesDefinitionsImporter, NoDefinitionsByInput)
{
   const commonItems::ModFilesystem mod_filesystem("test_files/vic3_world/cultures/no_definitions_by_default/game", {});
   const auto culture_definitions = ImportCultureDefinitions(mod_filesystem);

   EXPECT_TRUE(culture_definitions.empty());
}

TEST(Vic3WorldCountriesCountriesDefinitionsImporter, DefinitionsCanBeImported)
{
   const commonItems::ModFilesystem mod_filesystem("test_files/vic3_world/cultures/definitions_can_be_imported/game",
       {});
   const auto culture_definitions = ImportCultureDefinitions(mod_filesystem);

   CultureDefinition one(NameList{{"Wolfgang"},
                             {"Friederike"},
                             {"von_Jons"},
                             {"Olbers"},
                             std::make_optional(std::set<std::string>{"Oskar"}),
                             std::make_optional(std::set<std::string>{"Sophie"})},
       {"german_speaking", "european_heritage"},
       {"caucasian"});

   CultureDefinition two(NameList{{"Otto"},
                             {"Helmtrude"},
                             {"von_Xylander"},
                             {"Auerbach"},
                             std::make_optional(std::set<std::string>{"Lugwig"}),
                             std::make_optional(std::set<std::string>{"Charlotte"})},
       {"german_speaking", "european_heritage"},
       {"caucasian"});

   CultureDefinition three(NameList{{"Shadrach"}, {"Gwenllian"}, {"Insole"}, {"Talbot"}, std::nullopt, std::nullopt},
       {"anglophone", "celtic_people", "european_heritage"},
       {"caucasian", "welshy"});

   EXPECT_THAT(culture_definitions,
       testing::UnorderedElementsAre(testing::Pair("north_german", one)),
       testing::Pair("south_german", two),
       testing::Pair("welsh", three));
}

}  // namespace vic3