#include "external/commonItems/ModLoader/ModFilesystem.h"
#include "external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/googletest/googletest/include/gtest/gtest.h"
#include "src/vic3_world/countries/vic3_country.h"
#include "src/vic3_world/world/vic3_world_importer.h"



namespace vic3
{

TEST(Vic3WorldWorldVic3WorldImporter, ExceptionForMissingSave)
{
   EXPECT_THROW(ImportWorld("test_files/vic3_world/world/missing_save.vic3", commonItems::ModFilesystem("", {})),
       std::runtime_error);
}


TEST(Vic3WorldWorldVic3WorldImporter, ExceptionForIncorrectlyZippedSave)
{
   EXPECT_THROW(ImportWorld("test_files/vic3_world/world/bad_zipped_save.vic3", commonItems::ModFilesystem("", {})),
       std::runtime_error);
}


TEST(Vic3WorldWorldVic3WorldImporter, ExceptionForMissingGamestate)
{
   EXPECT_THROW(
       ImportWorld("test_files/vic3_world/world/missing_gamestate_save.vic3", commonItems::ModFilesystem("", {})),
       std::runtime_error);
}


TEST(Vic3WorldWorldVic3WorldImporter, UnzippedSizeIsLogged)
{
   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   ImportWorld("test_files/vic3_world/world/zipped_save.vic3",
       commonItems::ModFilesystem("test_files/vic3_world/world", {}));

   EXPECT_THAT(log.str(), testing::HasSubstr("Unzipped save to 42 bytes"));

   std::cout.rdbuf(cout_buffer);
}


TEST(Vic3WorldWorldVic3WorldImporter, DefaultsAreCorrect)
{
   const auto world = ImportWorld("test_files/vic3_world/world/empty_save.vic3",
       commonItems::ModFilesystem("test_files/vic3_world/empty_world", {}));

   EXPECT_TRUE(world.GetCountries().empty());
   EXPECT_TRUE(world.GetStates().empty());
   EXPECT_TRUE(world.GetProvinceDefinitions().GetProvinceDefinitions().empty());
}


TEST(Vic3WorldWorldVic3WorldImporter, WorldCanBeImported)
{
   const auto world = ImportWorld("test_files/vic3_world/world/test_save.vic3",
       commonItems::ModFilesystem("test_files/vic3_world/world", {}));

   EXPECT_THAT(world.GetCountries(),
       testing::UnorderedElementsAre(testing::Pair(1, Country("TAG")), testing::Pair(3, Country("TWO"))));
   EXPECT_THAT(world.GetStates(),
       testing::UnorderedElementsAre(testing::Pair(0, State({1, 2, 3})), testing::Pair(1, State({10, 11, 12}))));
}


}  // namespace vic3