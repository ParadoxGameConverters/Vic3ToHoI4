#include <filesystem>
#include <fstream>
#include <sstream>

#include "external/commonItems/OSCompatibilityLayer.h"
#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "external/fmt/include/fmt/format.h"
#include "src/out_hoi4/national_focus/out_focus_tree.h"



namespace out
{

TEST(Outhoi4NationalfocusOurfocustreeTests, NationalFocusFileIsGenericFocusTree)
{
   commonItems::TryCreateFolder("output");
   commonItems::TryCreateFolder("output/NationalFocusFileIsGenericFocusTree");
   commonItems::TryCreateFolder("output/NationalFocusFileIsGenericFocusTree/common");
   commonItems::TryCreateFolder("output/NationalFocusFileIsGenericFocusTree/common/national_focus");

   OutputFocusTree("NationalFocusFileIsGenericFocusTree", "TST");

   ASSERT_TRUE(
       commonItems::DoesFileExist("output/NationalFocusFileIsGenericFocusTree/common/national_focus/TST_NF.txt"));
   std::ifstream national_focus_file("output/NationalFocusFileIsGenericFocusTree/common/national_focus/TST_NF.txt");
   ASSERT_TRUE(national_focus_file.is_open());
   std::stringstream national_focus_file_stream;
   std::copy(std::istreambuf_iterator<char>(national_focus_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(national_focus_file_stream));
   national_focus_file.close();

   std::stringstream expected;
   expected << "focus_tree = {\n";
   expected << "\tid = TST_focus\n";
   expected << "\t\n";
   expected << "\tcountry = {\n";
   expected << "\t\tfactor = 0\n";
   expected << "\t\t\n";
   expected << "\t\tmodifier = {\n";
   expected << "\t\t\tadd = 10\n";
   expected << "\t\t\ttag = TST\n";
   expected << "\t\t}\n";
   expected << "\t}\n";
   expected << "\t\n";
   expected << "\tdefault = no\n";
   expected << "\treset_on_civilwar = no\n";
   expected << "\n";
   expected << "\tshared_focus = army_effort\n";
   expected << "\tshared_focus = aviation_effort\n";
   expected << "\tshared_focus = naval_effort\n";
   expected << "\tshared_focus = industrial_effort\n";
   expected << "\tshared_focus = political_effort\n";
   expected << "\n";
   expected << "}\n";
   EXPECT_EQ(national_focus_file_stream.str(), expected.str());
}

}  // namespace out