#include <filesystem>
#include <fstream>
#include <sstream>

#include "external/commonItems/OSCompatibilityLayer.h"
#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "external/fmt/include/fmt/format.h"
#include "src/out_hoi4/focus_trees/out_focus_tree.h"



namespace out
{

TEST(Outhoi4FocustreesOurfocustreeTests, NationalFocusFileIsNamedForTag)
{
   commonItems::TryCreateFolder("output");
   commonItems::TryCreateFolder("output/NationalFocusFileIsNamedForTag");
   commonItems::TryCreateFolder("output/NationalFocusFileIsNamedForTag/common");
   commonItems::TryCreateFolder("output/NationalFocusFileIsNamedForTag/common/national_focus");

   OutputFocusTree("NationalFocusFileIsNamedForTag", "TST", {});

   EXPECT_TRUE(commonItems::DoesFileExist("output/NationalFocusFileIsNamedForTag/common/national_focus/TST_NF.txt"));
}


TEST(Outhoi4FocustreesOurfocustreeTests, DefaultsAreOutput)
{
   commonItems::TryCreateFolder("output");
   commonItems::TryCreateFolder("output/DefaultsAreOutput");
   commonItems::TryCreateFolder("output/DefaultsAreOutput/common");
   commonItems::TryCreateFolder("output/DefaultsAreOutput/common/national_focus");

   OutputFocusTree("DefaultsAreOutput", "TST", {});

   ASSERT_TRUE(commonItems::DoesFileExist("output/DefaultsAreOutput/common/national_focus/TST_NF.txt"));
   std::ifstream national_focus_file("output/DefaultsAreOutput/common/national_focus/TST_NF.txt");
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
   expected << "\n";
   expected << "}\n";
   EXPECT_EQ(national_focus_file_stream.str(), expected.str());
}


TEST(Outhoi4FocustreesOurfocustreeTests, SharedFocusesAreOutput)
{
   commonItems::TryCreateFolder("output");
   commonItems::TryCreateFolder("output/SharedFocusesAreOutput");
   commonItems::TryCreateFolder("output/SharedFocusesAreOutput/common");
   commonItems::TryCreateFolder("output/SharedFocusesAreOutput/common/national_focus");

   OutputFocusTree("SharedFocusesAreOutput", "TST", {.shared_focuses = {"shared_focus_one", "shared_focus_two"}});

   ASSERT_TRUE(commonItems::DoesFileExist("output/SharedFocusesAreOutput/common/national_focus/TST_NF.txt"));
   std::ifstream national_focus_file("output/SharedFocusesAreOutput/common/national_focus/TST_NF.txt");
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
   expected << "\tshared_focus = shared_focus_one\n";
   expected << "\tshared_focus = shared_focus_two\n";
   expected << "\n";
   expected << "}\n";
   EXPECT_EQ(national_focus_file_stream.str(), expected.str());
}


TEST(Outhoi4FocustreesOurfocustreeTests, FocusesAreOutput)
{
   commonItems::TryCreateFolder("output");
   commonItems::TryCreateFolder("output/FocusesAreOutput");
   commonItems::TryCreateFolder("output/FocusesAreOutput/common");
   commonItems::TryCreateFolder("output/FocusesAreOutput/common/national_focus");

   OutputFocusTree("FocusesAreOutput", "TST", {.focuses = {{.id = "focus_one"}, {.id = "focus_two"}}});

   ASSERT_TRUE(commonItems::DoesFileExist("output/FocusesAreOutput/common/national_focus/TST_NF.txt"));
   std::ifstream national_focus_file("output/FocusesAreOutput/common/national_focus/TST_NF.txt");
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
   expected << "\tfocus = {\n";
   expected << "\t\tid = focus_one\n";
   expected << "\t\ticon = \n";
   expected << "\t\tx = 0\n";
   expected << "\t\ty = 0\n";
   expected << "\t\tcost = 0\n";
   expected << "\t\tcompletion_reward \n";
   expected << "\t}\n";
   expected << "\tfocus = {\n";
   expected << "\t\tid = focus_two\n";
   expected << "\t\ticon = \n";
   expected << "\t\tx = 0\n";
   expected << "\t\ty = 0\n";
   expected << "\t\tcost = 0\n";
   expected << "\t\tcompletion_reward \n";
   expected << "\t}\n";
   expected << "\n";
   expected << "}\n";
   EXPECT_EQ(national_focus_file_stream.str(), expected.str());
}

}  // namespace out