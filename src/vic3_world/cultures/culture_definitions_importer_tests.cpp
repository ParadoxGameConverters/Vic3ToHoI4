#include <external/commonItems/ModLoader/ModFilesystem.h>
#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

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
       NameList{
           .male_common_first = {"Wolfgang"},
           .female_common_first = {"Friederike"},
           .common_last = {"Olbers"},
           .noble_last = {"von_Jons"},
           .male_regal_first = {"Oskar"},
           .female_regal_first = {"Sophie"},
       },
       "protestant",
       {"german_speaking", "european_heritage"},
       {"tradition_order", "tradition_orderly"},
       {"obsession_rules", "obsession_punctuality"},
       "heritage_germanic",
       "language_germanophone",
       {"caucasian"});

   CultureDefinition two("south_german",
       NameList{
           .male_common_first = {"Otto"},
           .female_common_first = {"Helmtrude"},
           .common_last = {"Auerbach"},
           .noble_last = {"von_Xylander"},
           .male_regal_first = {"Lugwig"},
           .female_regal_first = {"Charlotte"},
       },
       "catholic",
       {"german_speaking", "european_heritage"},
       {"tradition_order", "tradition_orderly"},
       {"obsession_rules", "obsession_punctuality"},
       "heritage_germanic",
       "language_germanophone",
       {"caucasian"});

   CultureDefinition three("welsh",
       NameList{
           .male_common_first = {"Shadrach"},
           .female_common_first = {"Gwenllian"},
           .common_last = {"Insole"},
           .noble_last = {"Talbot"},
       },
       "protestant",
       {"anglophone", "celtic_people", "european_heritage"},
       {},
       {},
       "",
       "",
       {"caucasian", "welshy"});

   EXPECT_THAT(culture_definitions,
       testing::UnorderedElementsAre(testing::Pair("north_german", one),
           testing::Pair("south_german", two),
           testing::Pair("welsh", three)));
}

}  // namespace vic3