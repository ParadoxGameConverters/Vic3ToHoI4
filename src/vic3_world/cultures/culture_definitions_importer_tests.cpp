#include <sstream>

#include "external/commonItems/ModLoader/ModFilesystem.h"
#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/vic3_world/cultures/culture_definitions_importer.h"



namespace vic3
{

TEST(Vic3WorldCulturesCultureDefinitionsImporter, NoDefinitionsByDefault)
{
   const commonItems::ModFilesystem mod_filesystem("test_files/vic3_world/cultures/no_definitions_by_default/game", {});
   const auto culture_definitions = ImportCultureDefinitions(mod_filesystem);

   EXPECT_TRUE(culture_definitions.empty());
}

TEST(Vic3WorldCulturesCultureDefinitionsImporter, DefinitionsCanBeImported)
{
   const commonItems::ModFilesystem mod_filesystem("test_files/vic3_world/cultures/definitions_can_be_imported/game",
       {});
   const auto culture_definitions = ImportCultureDefinitions(mod_filesystem);

   CultureDefinition one("north_german",
       NameList{{"Wolfgang"}, {"Friederike"}, {"Olbers"}, {"von_Jons"}, {"Oskar"}, {"Sophie"}},
       {"german_speaking", "european_heritage"},
       {"caucasian"});

   CultureDefinition two("south_german",
       NameList{{"Otto"}, {"Helmtrude"}, {"Auerbach"}, {"von_Xylander"}, {"Lugwig"}, {"Charlotte"}},
       {"german_speaking", "european_heritage"},
       {"caucasian"});

   CultureDefinition three("welsh",
       NameList{{"Shadrach"}, {"Gwenllian"}, {"Insole"}, {"Talbot"}, {}, {}},
       {"anglophone", "celtic_people", "european_heritage"},
       {"caucasian", "welshy"});

   EXPECT_THAT(culture_definitions,
       testing::UnorderedElementsAre(testing::Pair("north_german", one),
           testing::Pair("south_german", two),
           testing::Pair("welsh", three)));
}

}  // namespace vic3