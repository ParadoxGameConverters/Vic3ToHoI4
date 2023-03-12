#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/vic3_world/states/state_regions_importer.h"



namespace vic3
{

TEST(Vic3worldStatesStateRegionsImporterTests, NoFilesNoRegions)
{
   const commonItems::ModFilesystem mod_filesystem("", {});
   const std::map<std::string, StateRegion> state_regions = ImportStateRegions(mod_filesystem);

   EXPECT_TRUE(state_regions.empty());
}


TEST(Vic3worldStatesStateRegionsImporterTests, ItemsAreImported)
{
   const commonItems::ModFilesystem mod_filesystem("test_files/vic3_world/states/ItemsAreImported", {});
   const std::map<std::string, StateRegion> state_regions = ImportStateRegions(mod_filesystem);

   EXPECT_THAT(state_regions,
       testing::UnorderedElementsAre(testing::Pair("STATE_LOMBARDY",
                                         StateRegion(std::map<std::string, std::string>{{"xD04060", "city"},
                                                         {"x867A90", "farm"},
                                                         {"x3F1E38", "mine"},
                                                         {"x4713EE", "wood"}},
                                             {"x3F1E38", "x4713EE"})),
           testing::Pair("STATE_PIEDMONT",
               StateRegion(std::map<std::string, std::string>{{"xA9397D", "city"},
                               {"x904061", "port"},
                               {"x11C061", "farm"},
                               {"x1140E0", "mine"},
                               {"xD080E0", "wood"}},
                   {"x1140E0", "x11C061"})),
           testing::Pair("STATE_SVEALAND",
               StateRegion(std::map<std::string, std::string>{{"x9686A5", "city"},
                               {"x93C3BC", "port"},
                               {"xF48646", "farm"},
                               {"x6F40EC", "mine"},
                               {"x4C9918", "wood"}},
                   {"x0974E5", "x216569"}))));
}

}  // namespace vic3