#include <external/commonItems/OSCompatibilityLayer.h>
#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>
#include <external/fmt/include/fmt/format.h>

#include <filesystem>
#include <fstream>
#include <sstream>

#include "src/out_hoi4/events/out_event_option.h"



namespace out
{

TEST(Outhoi4EventOuteventoptionTests, EmptyOptionIsOutput)
{
   const hoi4::EventOption option;

   std::ostringstream output;
   output << option;

   std::ostringstream expected;
   expected << "\toption = {\n";
   expected << "\t}";
   EXPECT_EQ(output.str(), expected.str());
}


TEST(Outhoi4EventOuteventoptionTests, ItemsAreOutput)
{
   const hoi4::EventOption option{
       .name = "option_name",
       .trigger =
           "= {\n"
           "\t\t\ttrigger_text\n"
           "\t\t}",
       .ai_chance = "= { factor = 0 }",
       .script_blocks = {"TAG = {\n"
                         "\t\t\tadd_to_faction = NEW\n"
                         "\t\t}",
           "TWO = {\n"
           "\t\t\tadd_to_faction = MOR\n"
           "\t\t}"},
       .hidden_effect =
           "= {\n"
           "\t\t\tnews_event = { id = news.42 }\n"
           "\t\t}",
   };

   std::ostringstream output;
   output << option;

   std::ostringstream expected;
   expected << "\toption = {\n";
   expected << "\t\tname = option_name\n";
   expected << "\t\ttrigger = {\n";
   expected << "\t\t\ttrigger_text\n";
   expected << "\t\t}\n";
   expected << "\t\tai_chance = { factor = 0 }\n";
   expected << "\t\tTAG = {\n";
   expected << "\t\t\tadd_to_faction = NEW\n";
   expected << "\t\t}\n";
   expected << "\t\tTWO = {\n";
   expected << "\t\t\tadd_to_faction = MOR\n";
   expected << "\t\t}\n";
   expected << "\t\thidden_effect = {\n";
   expected << "\t\t\tnews_event = { id = news.42 }\n";
   expected << "\t\t}\n";
   expected << "\t}";
   EXPECT_EQ(output.str(), expected.str());
}

}  // namespace out