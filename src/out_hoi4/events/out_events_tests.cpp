#include <external/commonItems/OSCompatibilityLayer.h>
#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>
#include <external/fmt/include/fmt/format.h>

#include <filesystem>
#include <fstream>
#include <sstream>

#include "src/hoi4_world/events/event.h"
#include "src/out_hoi4/events/out_events.h"



using std::filesystem::create_directories;
using std::filesystem::path;



namespace out
{

TEST(Outhoi4EventsOuteventsTests, EventFilesAreCreated)
{
   OutputEvents("EventFilesAreCreated",
       {
           {"TAG", {}},
           {"TWO", {}},
       });

   const path tag_path("output/EventFilesAreCreated/events/TAG.txt");
   EXPECT_TRUE(commonItems::DoesFileExist(tag_path));
   std::ifstream event_file_one(tag_path);
   ASSERT_TRUE(event_file_one.is_open());
   std::stringstream event_file_one_stream;
   std::copy(std::istreambuf_iterator<char>(event_file_one),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(event_file_one_stream));
   event_file_one.close();

   const path two_path("output/EventFilesAreCreated/events/TWO.txt");
   EXPECT_TRUE(commonItems::DoesFileExist(two_path));
   std::ifstream event_file_two(two_path);
   ASSERT_TRUE(event_file_two.is_open());
   std::stringstream event_file_two_stream;
   std::copy(std::istreambuf_iterator<char>(event_file_two),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(event_file_two_stream));
   event_file_two.close();
}


TEST(Outhoi4EventsOuteventsTests, ExceptionIfDirectoryNotCreated)
{
   EXPECT_THROW(OutputEvents("???",
                    {
                        {"TAG", {}},
                        {"TWO", {}},
                    }),
       std::runtime_error);
}


TEST(Outhoi4EventsOuteventsTests, ExceptionIfFileNotCreated)
{
   EXPECT_THROW(OutputEvents("ExceptionIfFileNotCreated",
                    {
                        {"???", {}},
                        {"TWO", {}},
                    }),
       std::runtime_error);
}

}  // namespace out
