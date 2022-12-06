#include <filesystem>
#include <fstream>
#include <sstream>

#include "external/commonItems/OSCompatibilityLayer.h"
#include "external/fmt/include/fmt/format.h"
#include "external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/googletest/googletest/include/gtest/gtest.h"
#include "src/hoi4_world/states/hoi4_state.h"
#include "src/out_hoi4/states/out_state.h"



namespace out
{


TEST(Outhoi4StatesState, ExceptionWhenFileNotOpened)
{
   const hoi4::State state_one(1, {});

   EXPECT_THROW(OutputState("ExceptionWhenFileNotOpened", state_one), std::runtime_error);
}


TEST(Outhoi4StatesState, StateFileIsNamedForId)
{
   commonItems::TryCreateFolder("output");
   commonItems::TryCreateFolder("output/StateFileIsNamedForId");
   commonItems::TryCreateFolder("output/StateFileIsNamedForId/history");
   commonItems::TryCreateFolder("output/StateFileIsNamedForId/history/states");

   const hoi4::State state_one(1, {});
   const hoi4::State state_two(2, {});

   OutputState("StateFileIsNamedForId", state_one);
   OutputState("StateFileIsNamedForId", state_two);

   EXPECT_TRUE(commonItems::DoesFileExist("output/StateFileIsNamedForId/history/states/1.txt"));
   EXPECT_TRUE(commonItems::DoesFileExist("output/StateFileIsNamedForId/history/states/2.txt"));
}


TEST(Outhoi4StatesState, BasicsAreOutput)
{
   commonItems::TryCreateFolder("output");
   commonItems::TryCreateFolder("output/BasicsAreOutput");
   commonItems::TryCreateFolder("output/BasicsAreOutput/history");
   commonItems::TryCreateFolder("output/BasicsAreOutput/history/states");

   const hoi4::State state_one(1, {});

   OutputState("BasicsAreOutput", state_one);

   ASSERT_TRUE(commonItems::DoesFileExist("output/StateFileIsNamedForId/history/states/1.txt"));
   std::ifstream state_file("output/StateFileIsNamedForId/history/states/1.txt");
   ASSERT_TRUE(state_file.is_open());
   std::stringstream state_file_stream;
   std::copy(std::istreambuf_iterator<char>(state_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(state_file_stream));
   state_file.close();
   EXPECT_EQ(state_file_stream.str(),
       "state = {\n"
       "\tid = 1\n"
       "\tname = \"STATE_1\"\n"
       "\tmanpower = 1000\n"
       "\n"
       "\tstate_category = rural\n"
       "\n"
       "\tprovinces = {\n"
       "\t\t\n"
       "\t}\n"
       "}\n");
}


TEST(Outhoi4StatesState, IdIsSetById)
{
   commonItems::TryCreateFolder("output");
   commonItems::TryCreateFolder("output/IdIsSetById");
   commonItems::TryCreateFolder("output/IdIsSetById/history");
   commonItems::TryCreateFolder("output/IdIsSetById/history/states");

   const hoi4::State state_one(1, {});
   const hoi4::State state_two(2, {});

   OutputState("IdIsSetById", state_one);
   OutputState("IdIsSetById", state_two);

   ASSERT_TRUE(commonItems::DoesFileExist("output/IdIsSetById/history/states/1.txt"));
   std::ifstream state_file_one("output/IdIsSetById/history/states/1.txt");
   ASSERT_TRUE(state_file_one.is_open());
   std::stringstream state_file_stream_one;
   std::copy(std::istreambuf_iterator<char>(state_file_one),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(state_file_stream_one));
   state_file_one.close();
   EXPECT_THAT(state_file_stream_one.str(), testing::HasSubstr("id = 1"));

   ASSERT_TRUE(commonItems::DoesFileExist("output/IdIsSetById/history/states/2.txt"));
   std::ifstream state_file_two("output/IdIsSetById/history/states/2.txt");
   ASSERT_TRUE(state_file_two.is_open());
   std::stringstream state_file_stream_two;
   std::copy(std::istreambuf_iterator<char>(state_file_two),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(state_file_stream_two));
   state_file_two.close();
   EXPECT_THAT(state_file_stream_two.str(), testing::HasSubstr("id = 2"));
}


TEST(Outhoi4StatesState, NameIsSetById)
{
   commonItems::TryCreateFolder("output");
   commonItems::TryCreateFolder("output/NameIsSetById");
   commonItems::TryCreateFolder("output/NameIsSetById/history");
   commonItems::TryCreateFolder("output/NameIsSetById/history/states");

   const hoi4::State state_one(1, {});
   const hoi4::State state_two(2, {});

   OutputState("NameIsSetById", state_one);
   OutputState("NameIsSetById", state_two);

   ASSERT_TRUE(commonItems::DoesFileExist("output/NameIsSetById/history/states/1.txt"));
   std::ifstream state_file_one("output/NameIsSetById/history/states/1.txt");
   ASSERT_TRUE(state_file_one.is_open());
   std::stringstream state_file_stream_one;
   std::copy(std::istreambuf_iterator<char>(state_file_one),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(state_file_stream_one));
   state_file_one.close();
   EXPECT_THAT(state_file_stream_one.str(), testing::HasSubstr("name = \"STATE_1\""));

   ASSERT_TRUE(commonItems::DoesFileExist("output/NameIsSetById/history/states/2.txt"));
   std::ifstream state_file_two("output/NameIsSetById/history/states/2.txt");
   ASSERT_TRUE(state_file_two.is_open());
   std::stringstream state_file_stream_two;
   std::copy(std::istreambuf_iterator<char>(state_file_two),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(state_file_stream_two));
   state_file_two.close();
   EXPECT_THAT(state_file_stream_two.str(), testing::HasSubstr("name = \"STATE_2\""));
}


TEST(Outhoi4StatesState, ProvincesAreOutput)
{
   commonItems::TryCreateFolder("output");
   commonItems::TryCreateFolder("output/ProvincesAreOutput");
   commonItems::TryCreateFolder("output/ProvincesAreOutput/history");
   commonItems::TryCreateFolder("output/ProvincesAreOutput/history/states");

   const hoi4::State state_one(1, {1, 4, 9, 16});

   OutputState("ProvincesAreOutput", state_one);

   ASSERT_TRUE(commonItems::DoesFileExist("output/ProvincesAreOutput/history/states/1.txt"));
   std::ifstream state_file("output/ProvincesAreOutput/history/states/1.txt");
   ASSERT_TRUE(state_file.is_open());
   std::stringstream state_file_stream;
   std::copy(std::istreambuf_iterator<char>(state_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(state_file_stream));
   state_file.close();
   EXPECT_THAT(state_file_stream.str(),
       testing::HasSubstr("\tprovinces = {\n"
                          "\t\t1 4 9 16 \n"
                          "\t}\n"));
}

}  // namespace out