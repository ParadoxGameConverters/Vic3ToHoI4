#include <external/commonItems/OSCompatibilityLayer.h>
#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <filesystem>
#include <sstream>

#include "src/out_hoi4/names/out_name_list.h"


namespace out
{


TEST(Outhoi4NameLists, NamesAreOutput)
{
   const hoi4::NameList name_list({"male"}, {"female"}, {"surname"}, {"m_surname"}, {"f_surname"});

   std::stringstream out;
   out << name_list;

   EXPECT_EQ(out.str(),
       "\tmale = {\n"
       "\t\tnames = {\n"
       "\t\t\t\"male\" \n"
       "\t\t}\n"
       "\t\tsurnames = {\n"
       "\t\t\t\"m_surname\" \n"
       "\t\t}\n"
       "\t}\n"
       "\tfemale = {\n"
       "\t\tnames = {\n"
       "\t\t\t\"female\" \n"
       "\t\t}\n"
       "\t\tsurnames = {\n"
       "\t\t\t\"f_surname\" \n"
       "\t\t}\n"
       "\t}\n"
       "\tsurnames = {\n"
       "\t\t\t\"surname\" \n"
       "\t}\n");
}


TEST(Outhoi4NameLists, NamesWrap)
{
   const hoi4::NameList name_list({"m0", "m1", "m2", "m3", "m4", "m5", "m6", "m7", "m8", "m9", "m10", "m11"},
       {},
       {},
       {},
       {});

   std::stringstream out;
   out << name_list;

   EXPECT_EQ(out.str(),
       "\tmale = {\n"
       "\t\tnames = {\n"
       "\t\t\t\"m0\" \"m1\" \"m10\" \"m11\" \"m2\" \"m3\" \"m4\" \"m5\" \"m6\" \"m7\" \"m8\" \n"
       "\t\t\t\"m9\" \n"
       "\t\t}\n"
       "\t}\n"
       "\tfemale = {\n"
       "\t\tnames = {\n"
       "\t\t\t\n"
       "\t\t}\n"
       "\t}\n"
       "\tsurnames = {\n"
       "\t\t\t\n"
       "\t}\n");
}


TEST(Outhoi4NameLists, EmptySpecificSurnamesAreOmitted)
{
   const hoi4::NameList name_list({}, {}, {}, {}, {});

   std::stringstream out;
   out << name_list;

   EXPECT_EQ(out.str(),
       "\tmale = {\n"
       "\t\tnames = {\n"
       "\t\t\t\n"
       "\t\t}\n"
       "\t}\n"
       "\tfemale = {\n"
       "\t\tnames = {\n"
       "\t\t\t\n"
       "\t\t}\n"
       "\t}\n"
       "\tsurnames = {\n"
       "\t\t\t\n"
       "\t}\n");
}

}  // namespace out