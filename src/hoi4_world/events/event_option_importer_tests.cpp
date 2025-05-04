#include <sstream>

#include "event_option_importer.h"
#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"



namespace hoi4
{

TEST(Hoi4worldEventsEventOptionimporterTests, ItemsAreDefaulted)
{
   std::stringstream input;

   EventOptionImporter importer;
   const EventOption option = importer.ImportOption(input);

   EXPECT_TRUE(option.name.empty());
   EXPECT_TRUE(option.trigger.empty());
   EXPECT_TRUE(option.ai_chance.empty());
   EXPECT_TRUE(option.script_blocks.empty());
   EXPECT_TRUE(option.hidden_effect.empty());
}


TEST(Hoi4worldEventsEventOptionimporterTests, ItemsCanBeImported)
{
   std::stringstream input;
   input << "= {\n";
   input << "\tname = option_name\n";
   input << "\ttrigger = {\n";
   input << "\t\ttrigger_text\n";
   input << "\t}\n";
   input << "\tai_chance = { factor = 0 }\n";
   input << "\tTAG = {\n";
   input << "\t\tadd_to_faction = NEW\n";
   input << "\t}";
   input << "\tTWO = {\n";
   input << "\t\tadd_to_faction = MOR\n";
   input << "\t}";
   input << "\thidden_effect = {\n";
   input << "\t\tnews_event = { id = news.42 }\n";
   input << "\t}\n";
   input << "}";

   EventOptionImporter importer;
   const EventOption option = importer.ImportOption(input);

   EXPECT_EQ(option.name, "option_name");
   EXPECT_EQ(option.trigger,
       "= {\n"
       "\t\ttrigger_text\n"
       "\t}");
   EXPECT_EQ(option.ai_chance, "= { factor = 0 }");
   EXPECT_THAT(option.script_blocks,
       testing::ElementsAre("TAG = {\n"
                            "\t\tadd_to_faction = NEW\n"
                            "\t}\n",
           "TWO = {\n"
           "\t\tadd_to_faction = MOR\n"
           "\t}\n"));
   EXPECT_EQ(option.hidden_effect,
       "= {\n"
       "\t\tnews_event = { id = news.42 }\n"
       "\t}");
}

}  // namespace hoi4