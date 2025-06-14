#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

#include "src/hoi4_world/events/event.h"
#include "src/hoi4_world/events/event_importer.h"



namespace hoi4
{

TEST(Hoi4worldEventsEventImporterTests, ItemsAreDefaulted)
{
   std::stringstream input;
   const Event event = EventImporter{}.ImportEvent("test_type", input);

   EXPECT_EQ(event.type, "test_type");
   EXPECT_TRUE(event.id.empty());
   EXPECT_FALSE(event.title.has_value());
   EXPECT_TRUE(event.descriptions.empty());
   EXPECT_FALSE(event.picture.has_value());
   EXPECT_FALSE(event.major_event);
   EXPECT_FALSE(event.triggered_only);
   EXPECT_FALSE(event.hidden.has_value());
   EXPECT_TRUE(event.trigger.empty());
   EXPECT_FALSE(event.fire_only_once.has_value());
   EXPECT_TRUE(event.mean_time_to_happen.empty());
   EXPECT_TRUE(event.immediate.empty());
   EXPECT_TRUE(event.options.empty());
}


TEST(Hoi4worldEventsEventImporterTests, ItemsCanBeInput)
{
   std::stringstream input;
   input << "= {\n";
   input << "\tid = test_id\n";
   input << "\ttitle = event_title\n";
   input << "\tdesc = event_description_one\n";
   input << "\tdesc = event_description_two\n";
   input << "\tpicture = event_picture\n";
   input << "\tmajor = yes\n";
   input << "\tis_triggered_only = yes\n";
   input << "\thidden = yes\n";
   input << "\ttrigger = {\n";
   input << "\t\tkey = value\n";
   input << "\t}\n";
   input << "\tfire_only_once = yes\n";
   input << "\tmean_time_to_happen = {\n";
   input << "\t\tdays = 150\n";
   input << "\t}\n";
   input << "\timmediate = {\n";
   input << "\t\tkey = value\n";
   input << "\t}\n";
   input << "\toption = {\n";
   input << "\t\tname = option_one\n";
   input << "\t}";
   input << "\toption = {\n";
   input << "\t\tname = option_two\n";
   input << "\t}";
   input << "}";
   const Event event = EventImporter{}.ImportEvent("test_type", input);

   EXPECT_EQ(event.id, "test_id");
   EXPECT_EQ(event.title, "event_title");
   EXPECT_THAT(event.descriptions, testing::ElementsAre("= event_description_one", "= event_description_two"));
   EXPECT_TRUE(event.picture.has_value());
   EXPECT_EQ(event.picture.value_or(""), "event_picture");
   EXPECT_TRUE(event.major_event);
   EXPECT_TRUE(event.triggered_only);
   EXPECT_TRUE(event.hidden.has_value());
   EXPECT_TRUE(event.hidden.value_or(false));
   EXPECT_EQ(event.trigger,
       "= {\n"
       "\t\tkey = value\n"
       "\t}");
   EXPECT_TRUE(event.fire_only_once.has_value());
   EXPECT_TRUE(event.fire_only_once.value_or(false));
   EXPECT_EQ(event.mean_time_to_happen,
       "= {\n"
       "\t\tdays = 150\n"
       "\t}");
   EXPECT_EQ(event.immediate,
       "= {\n"
       "\t\tkey = value\n"
       "\t}");
   EXPECT_THAT(event.options,
       testing::ElementsAre(EventOption{.name = "option_one"}, EventOption{.name = "option_two"}));
}


TEST(Hoi4worldEventsEventImporterTests, BoolsCanBeSetToFalse)
{
   std::stringstream input;
   input << "= {\n";
   input << "\tmajor = foo\n";
   input << "\tis_triggered_only = bar\n";
   input << "\thidden = baz\n";
   input << "\tfire_only_once = qux\n";
   input << "}";
   const Event event = EventImporter{}.ImportEvent("test_type", input);

   EXPECT_FALSE(event.major_event);
   EXPECT_FALSE(event.triggered_only);
   EXPECT_TRUE(event.hidden.has_value());
   EXPECT_FALSE(event.hidden.value_or(true));
   EXPECT_TRUE(event.fire_only_once.has_value());
   EXPECT_FALSE(event.fire_only_once.value_or(true));
}

}  // namespace hoi4