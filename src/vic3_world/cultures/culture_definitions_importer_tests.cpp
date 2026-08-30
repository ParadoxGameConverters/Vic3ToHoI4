#include <external/commonItems/ModLoader/ModFilesystem.h>
#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

#include "src/vic3_world/cultures/culture_definitions_importer.h"



namespace vic3
{

TEST(Vic3WorldCulturesCultureDefinitionsImporter, NoDefinitionsByDefault)  // NOLINT(cert-err58-cpp)
{
   const commonItems::ModFilesystem mod_filesystem("test_files/vic3_world/cultures/no_definitions_by_default/game", {});
   const auto culture_definitions = ImportCultureDefinitions(mod_filesystem);

   EXPECT_TRUE(culture_definitions.empty());
}


TEST(Vic3WorldCulturesCultureDefinitionsImporter, DefinitionsCanBeImported)  // NOLINT(cert-err58-cpp)
{
   const commonItems::ModFilesystem mod_filesystem("test_files/vic3_world/cultures/definitions_can_be_imported/game",
       {});
   const auto culture_definitions = ImportCultureDefinitions(mod_filesystem);

   CultureDefinition one({
       .name = "north_german",
       .name_list =
           NameList{
               .male_common_first = {"Wolfgang"},
               .female_common_first = {"Friederike"},
               .common_last = {"Olbers"},
               .noble_last = {"von_Jons"},
               .male_regal_first = {"Oskar"},
               .female_regal_first = {"Sophie"},
           },
       .religion = "protestant",
       .traits = {"german_speaking", "european_heritage"},
       .traditions = {"tradition_order", "tradition_orderly"},
       .obsessions = {"obsession_rules", "obsession_punctuality"},
       .heritage = "heritage_germanic",
       .language = "language_germanophone",
       .ethnicities = {"caucasian"},
   });

   CultureDefinition two({
       .name = "south_german",
       .name_list =
           NameList{
               .male_common_first = {"Otto"},
               .female_common_first = {"Helmtrude"},
               .common_last = {"Auerbach"},
               .noble_last = {"von_Xylander"},
               .male_regal_first = {"Lugwig"},
               .female_regal_first = {"Charlotte"},
           },
       .religion = "catholic",
       .traits = {"german_speaking", "european_heritage"},
       .traditions = {"tradition_order", "tradition_orderly"},
       .obsessions = {"obsession_rules", "obsession_punctuality"},
       .heritage = "heritage_germanic",
       .language = "language_germanophone",
       .ethnicities = {"caucasian"},
   });

   CultureDefinition three({
       .name = "welsh",
       .name_list =
           NameList{
               .male_common_first = {"Shadrach"},
               .female_common_first = {"Gwenllian"},
               .common_last = {"Insole"},
               .noble_last = {"Talbot"},
           },
       .religion = "protestant",
       .traits = {"anglophone", "celtic_people", "european_heritage"},
       .traditions = {},
       .obsessions = {},
       .heritage = "",
       .language = "",
       .ethnicities = {"caucasian", "welshy"},
   });

   EXPECT_THAT(culture_definitions,
       testing::UnorderedElementsAre(testing::Pair("north_german", one),
           testing::Pair("south_german", two),
           testing::Pair("welsh", three)));
}

}  // namespace vic3