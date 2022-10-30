#include "external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/googletest/googletest/include/gtest/gtest.h"
#include "src/vic3_world/world/vic3_world_importer.h"



namespace vic3
{

TEST(Vic3WorldWorldVic3WorldImporter, ExceptionForMissingSave)
{
   EXPECT_THROW(ImportWorld("test_files/vic3_world/world/missing_save.vic3"), std::runtime_error);
}

TEST(Vic3WorldWorldVic3WorldImporter, ExceptionForIncorrectlyZippedSave)
{
   EXPECT_THROW(ImportWorld("test_files/vic3_world/world/bad_zipped_save.vic3"), std::runtime_error);
}


TEST(Vic3WorldWorldVic3WorldImporter, ExceptionForMissingGamestate)
{
   EXPECT_THROW(ImportWorld("test_files/vic3_world/world/missing_gamestate_save.vic3"), std::runtime_error);
}


TEST(Vic3WorldWorldVic3WorldImporter, UnzippedSizeIsLogged)
{
   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   ImportWorld("test_files/vic3_world/world/zipped_save.vic3");

   EXPECT_THAT(log.str(), testing::HasSubstr("Unzipped save to 42 bytes"));

   std::cout.rdbuf(cout_buffer);
}


}  // namespace vic3