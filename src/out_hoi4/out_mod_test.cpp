#include "out_mod.h"

#include <fstream>
#include <sstream>

#include "external/commonItems/OSCompatibilityLayer.h"
#include "external/googletest/googletest/include/gtest/gtest.h"



TEST(OutHoi4OutModTest, ModFolderIsCreated)
{
   output("test_output");

   EXPECT_TRUE(commonItems::DoesFolderExist("output/test_output"));
}