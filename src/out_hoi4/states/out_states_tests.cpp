#include <external/commonItems/OSCompatibilityLayer.h>
#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>
#include <external/fmt/include/fmt/format.h>

#include <filesystem>
#include <fstream>
#include <sstream>

#include "src/hoi4_world/states/hoi4_state.h"
#include "src/out_hoi4/states/out_states.h"



using std::filesystem::create_directories;
using std::filesystem::path;



namespace out
{

TEST(Outhoi4StatesStates, StatesAreOutput)
{
   create_directories("output/StatesAreOutput/history/states");

   const hoi4::State state_one(1, {});
   const hoi4::State state_two(2, {});

   OutputStates("StatesAreOutput", {state_one, state_two});

   EXPECT_TRUE(commonItems::DoesFileExist(path("output/StatesAreOutput/history/states/1.txt")));
   EXPECT_TRUE(commonItems::DoesFileExist(path("output/StatesAreOutput/history/states/2.txt")));
}

}  // namespace out