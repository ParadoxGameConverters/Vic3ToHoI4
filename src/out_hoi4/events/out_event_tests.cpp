#include <external/commonItems/OSCompatibilityLayer.h>
#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

#include "src/out_hoi4/events/out_event.h"



namespace out
{

TEST(Outhoi4EventOuteventTests, EmptyEventIsOutput)
{
   const hoi4::Event event{.type = "test_type", .id = "test_id"};

   std::ostringstream output;
   output << event;

   std::ostringstream expected;
   expected << "test_type = {\n";
   expected << "\tid = test_id\n";
   expected << "\n";
   expected << "}\n";
   EXPECT_EQ(output.str(), expected.str());
}


TEST(Outhoi4EventOuteventTests, ItemsAreOutput)
{
   const hoi4::Event event{
       .type = "test_type",
       .id = "test_id",
       .title = "test_title",
       .descriptions = {"= description_one", "= description_two"},
       .picture = "test_picture",
       .major_event = true,
       .triggered_only = true,
       .hidden = true,
       .trigger =
           "= {\n"
           "\t\tkey = value\n"
           "\t}",
       .fire_only_once = true,
       .mean_time_to_happen =
           "= {\n"
           "\t\tdays = 150\n"
           "\t}",
       .immediate =
           "= {\n"
           "\t\tkey = value\n"
           "\t}",
       .options = {{.name = "option_one"}, {.name = "option_two"}},
   };

   std::ostringstream output;
   output << event;

   std::ostringstream expected;
   expected << "test_type = {\n";
   expected << "\tid = test_id\n";
   expected << "\ttitle = test_title\n";
   expected << "\tdesc = description_one\n";
   expected << "\tdesc = description_two\n";
   expected << "\tpicture = test_picture\n";
   expected << "\t\n";
   expected << "\tmajor = yes\n";
   expected << "\n";
   expected << "\tis_triggered_only = yes\n";
   expected << "\thidden = yes\n";
   expected << "\n";
   expected << "\ttrigger = {\n";
   expected << "\t\tkey = value\n";
   expected << "\t}\n";
   expected << "\n";
   expected << "\tfire_only_once = yes\n";
   expected << "\n";
   expected << "\tmean_time_to_happen = {\n"
               "\t\tdays = 150\n"
               "\t}\n";
   expected << "\n";
   expected << "\timmediate = {\n"
               "\t\tkey = value\n"
               "\t}\n";
   expected << "\n";
   expected << "\toption = {\n";
   expected << "\t\tname = option_one\n";
   expected << "\t}\n";
   expected << "\n";
   expected << "\toption = {\n";
   expected << "\t\tname = option_two\n";
   expected << "\t}\n";
   expected << "}\n";
   EXPECT_EQ(output.str(), expected.str());
}


TEST(Outhoi4EventOuteventTests, FalseItemsAreOutput)
{
   const hoi4::Event event{.type = "test_type", .id = "test_id", .hidden = false, .fire_only_once = false};

   std::ostringstream output;
   output << event;

   std::ostringstream expected;
   expected << "test_type = {\n";
   expected << "\tid = test_id\n";
   expected << "\n";
   expected << "\thidden = no\n";
   expected << "\n";
   expected << "\tfire_only_once = no\n";
   expected << "}\n";
   EXPECT_EQ(output.str(), expected.str());
}

}  // namespace out