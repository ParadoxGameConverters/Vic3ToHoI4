#include <filesystem>
#include <fstream>
#include <sstream>

#include "external/commonItems/OSCompatibilityLayer.h"
#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "external/fmt/include/fmt/format.h"
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
       "\tmanpower = 0\n"
       "\n"
       "\tstate_category = rural\n"
       "\n"
       "\thistory = {\n"
       "\t\tbuildings = {\n"
       "\t\t\tindustrial_complex = 0\n"
       "\t\t\tarms_factory = 0\n"
       "\t\t}\n"
       "\t}\n"
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


TEST(Outhoi4StatesState, ManpowerIsSetByManpower)
{
   commonItems::TryCreateFolder("output");
   commonItems::TryCreateFolder("output/manpowerIsSetByManpower");
   commonItems::TryCreateFolder("output/manpowerIsSetByManpower/history");
   commonItems::TryCreateFolder("output/manpowerIsSetByManpower/history/states");

   const hoi4::State state_one(1, {.manpower = 12345});
   const hoi4::State state_two(2, {.manpower = 67890});

   OutputState("manpowerIsSetByManpower", state_one);
   OutputState("manpowerIsSetByManpower", state_two);

   ASSERT_TRUE(commonItems::DoesFileExist("output/manpowerIsSetByManpower/history/states/1.txt"));
   std::ifstream state_file_one("output/manpowerIsSetByManpower/history/states/1.txt");
   ASSERT_TRUE(state_file_one.is_open());
   std::stringstream state_file_stream_one;
   std::copy(std::istreambuf_iterator<char>(state_file_one),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(state_file_stream_one));
   state_file_one.close();
   EXPECT_THAT(state_file_stream_one.str(), testing::HasSubstr("manpower = 12345"));

   ASSERT_TRUE(commonItems::DoesFileExist("output/manpowerIsSetByManpower/history/states/2.txt"));
   std::ifstream state_file_two("output/manpowerIsSetByManpower/history/states/2.txt");
   ASSERT_TRUE(state_file_two.is_open());
   std::stringstream state_file_stream_two;
   std::copy(std::istreambuf_iterator<char>(state_file_two),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(state_file_stream_two));
   state_file_two.close();
   EXPECT_THAT(state_file_stream_two.str(), testing::HasSubstr("manpower = 67890"));
}


TEST(Outhoi4StatesState, NoResourcesMeansNoResourcesSection)
{
   commonItems::TryCreateFolder("output");
   commonItems::TryCreateFolder("output/NoResourcesMeansNoResourcesSection");
   commonItems::TryCreateFolder("output/NoResourcesMeansNoResourcesSection/history");
   commonItems::TryCreateFolder("output/NoResourcesMeansNoResourcesSection/history/states");

   const hoi4::State state_one(1, {});
   const hoi4::State state_two(2, {});

   OutputState("NoResourcesMeansNoResourcesSection", state_one);
   OutputState("NoResourcesMeansNoResourcesSection", state_two);

   ASSERT_TRUE(commonItems::DoesFileExist("output/NoResourcesMeansNoResourcesSection/history/states/1.txt"));
   std::ifstream state_file_one("output/NoResourcesMeansNoResourcesSection/history/states/1.txt");
   ASSERT_TRUE(state_file_one.is_open());
   std::stringstream state_file_stream_one;
   std::copy(std::istreambuf_iterator<char>(state_file_one),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(state_file_stream_one));
   state_file_one.close();
   EXPECT_THAT(state_file_stream_one.str(),
       testing::HasSubstr("manpower = 0\n"
                          "\n"
                          "\tstate_category = rural"));

   ASSERT_TRUE(commonItems::DoesFileExist("output/NoResourcesMeansNoResourcesSection/history/states/2.txt"));
   std::ifstream state_file_two("output/NoResourcesMeansNoResourcesSection/history/states/2.txt");
   ASSERT_TRUE(state_file_two.is_open());
   std::stringstream state_file_stream_two;
   std::copy(std::istreambuf_iterator<char>(state_file_two),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(state_file_stream_two));
   state_file_two.close();
   EXPECT_THAT(state_file_stream_two.str(),
       testing::HasSubstr("manpower = 0\n"
                          "\n"
                          "\tstate_category = rural"));
}



TEST(Outhoi4StatesState, ResourcesAreOutput)
{
   commonItems::TryCreateFolder("output");
   commonItems::TryCreateFolder("output/NoResourcesMeansNoResourcesSection");
   commonItems::TryCreateFolder("output/NoResourcesMeansNoResourcesSection/history");
   commonItems::TryCreateFolder("output/NoResourcesMeansNoResourcesSection/history/states");

   const hoi4::State state_one(1, {.resources = {{"resource_one", 2.0}}});
   const hoi4::State state_two(2, {.resources = {{"resource_one", 3.0}, {"resource_two", 5.0}}});

   OutputState("NoResourcesMeansNoResourcesSection", state_one);
   OutputState("NoResourcesMeansNoResourcesSection", state_two);

   ASSERT_TRUE(commonItems::DoesFileExist("output/NoResourcesMeansNoResourcesSection/history/states/1.txt"));
   std::ifstream state_file_one("output/NoResourcesMeansNoResourcesSection/history/states/1.txt");
   ASSERT_TRUE(state_file_one.is_open());
   std::stringstream state_file_stream_one;
   std::copy(std::istreambuf_iterator<char>(state_file_one),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(state_file_stream_one));
   state_file_one.close();
   EXPECT_THAT(state_file_stream_one.str(),
       testing::HasSubstr("resources = {\n"
                          "\t\tresource_one = 2\n"
                          "\t}"));

   ASSERT_TRUE(commonItems::DoesFileExist("output/NoResourcesMeansNoResourcesSection/history/states/2.txt"));
   std::ifstream state_file_two("output/NoResourcesMeansNoResourcesSection/history/states/2.txt");
   ASSERT_TRUE(state_file_two.is_open());
   std::stringstream state_file_stream_two;
   std::copy(std::istreambuf_iterator<char>(state_file_two),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(state_file_stream_two));
   state_file_two.close();
   EXPECT_THAT(state_file_stream_two.str(),
       testing::HasSubstr("resources = {\n"
                          "\t\tresource_one = 3\n"
                          "\t\tresource_two = 5\n"
                          "\t}"));
}


TEST(Outhoi4StatesState, CategoryIsSetByCategory)
{
   commonItems::TryCreateFolder("output");
   commonItems::TryCreateFolder("output/CategoryIsSetByCategory");
   commonItems::TryCreateFolder("output/CategoryIsSetByCategory/history");
   commonItems::TryCreateFolder("output/CategoryIsSetByCategory/history/states");

   const hoi4::State state_one(1, {.category = "category_one"});
   const hoi4::State state_two(2, {.category = "category_two"});

   OutputState("CategoryIsSetByCategory", state_one);
   OutputState("CategoryIsSetByCategory", state_two);

   ASSERT_TRUE(commonItems::DoesFileExist("output/CategoryIsSetByCategory/history/states/1.txt"));
   std::ifstream state_file_one("output/CategoryIsSetByCategory/history/states/1.txt");
   ASSERT_TRUE(state_file_one.is_open());
   std::stringstream state_file_stream_one;
   std::copy(std::istreambuf_iterator<char>(state_file_one),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(state_file_stream_one));
   state_file_one.close();
   EXPECT_THAT(state_file_stream_one.str(), testing::HasSubstr("state_category = category_one"));

   ASSERT_TRUE(commonItems::DoesFileExist("output/CategoryIsSetByCategory/history/states/2.txt"));
   std::ifstream state_file_two("output/CategoryIsSetByCategory/history/states/2.txt");
   ASSERT_TRUE(state_file_two.is_open());
   std::stringstream state_file_stream_two;
   std::copy(std::istreambuf_iterator<char>(state_file_two),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(state_file_stream_two));
   state_file_two.close();
   EXPECT_THAT(state_file_stream_two.str(), testing::HasSubstr("state_category = category_two"));
}


TEST(Outhoi4StatesState, WastelandsAreImpassable)
{
   commonItems::TryCreateFolder("output");
   commonItems::TryCreateFolder("output/WastelandsAreImpassable");
   commonItems::TryCreateFolder("output/WastelandsAreImpassable/history");
   commonItems::TryCreateFolder("output/WastelandsAreImpassable/history/states");

   const hoi4::State state_one(1, {.category = "wasteland"});

   OutputState("WastelandsAreImpassable", state_one);

   ASSERT_TRUE(commonItems::DoesFileExist("output/WastelandsAreImpassable/history/states/1.txt"));
   std::ifstream state_file_one("output/WastelandsAreImpassable/history/states/1.txt");
   ASSERT_TRUE(state_file_one.is_open());
   std::stringstream state_file_stream_one;
   std::copy(std::istreambuf_iterator<char>(state_file_one),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(state_file_stream_one));
   state_file_one.close();
   EXPECT_THAT(state_file_stream_one.str(), testing::HasSubstr("impassable = yes"));
}


TEST(Outhoi4StatesState, ProvincesAreOutput)
{
   commonItems::TryCreateFolder("output");
   commonItems::TryCreateFolder("output/ProvincesAreOutput");
   commonItems::TryCreateFolder("output/ProvincesAreOutput/history");
   commonItems::TryCreateFolder("output/ProvincesAreOutput/history/states");

   const hoi4::State state_one(1, {.provinces = {1, 4, 9, 16}});

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

TEST(Outhoi4StatesState, OwnerIsOutput)
{
   commonItems::TryCreateFolder("output");
   commonItems::TryCreateFolder("output/ProvincesAreOutput");
   commonItems::TryCreateFolder("output/ProvincesAreOutput/history");
   commonItems::TryCreateFolder("output/ProvincesAreOutput/history/states");

   const hoi4::State state_one(1, {.owner = "TAG", .provinces = {1, 4, 9, 16}});

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
       testing::HasSubstr("\thistory = {\n"
                          "\t\towner = TAG\n"));
}


TEST(Outhoi4StatesState, VictoryPointsAreOutput)
{
   commonItems::TryCreateFolder("output");
   commonItems::TryCreateFolder("output/VictoryPointsAreOutput");
   commonItems::TryCreateFolder("output/VictoryPointsAreOutput/history");
   commonItems::TryCreateFolder("output/VictoryPointsAreOutput/history/states");

   const hoi4::State state_one(1, {.owner = "TAG", .victory_points = {{1, 1}, {2, 4}, {3, 9}}});

   OutputState("VictoryPointsAreOutput", state_one);

   ASSERT_TRUE(commonItems::DoesFileExist("output/VictoryPointsAreOutput/history/states/1.txt"));
   std::ifstream state_file("output/VictoryPointsAreOutput/history/states/1.txt");
   ASSERT_TRUE(state_file.is_open());
   std::stringstream state_file_stream;
   std::copy(std::istreambuf_iterator<char>(state_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(state_file_stream));
   state_file.close();
   EXPECT_THAT(state_file_stream.str(),
       testing::HasSubstr("\t\tvictory_points = {\n"
                          "\t\t\t1 1\n"
                          "\t\t}\n"));
   EXPECT_THAT(state_file_stream.str(),
       testing::HasSubstr("\t\tvictory_points = {\n"
                          "\t\t\t2 4\n"
                          "\t\t}\n"));
   EXPECT_THAT(state_file_stream.str(),
       testing::HasSubstr("\t\tvictory_points = {\n"
                          "\t\t\t3 9\n"
                          "\t\t}\n"));
}


TEST(Outhoi4StatesState, BuildingsAreOutput)
{
   commonItems::TryCreateFolder("output");
   commonItems::TryCreateFolder("output/BuildingsAreOutput");
   commonItems::TryCreateFolder("output/BuildingsAreOutput/history");
   commonItems::TryCreateFolder("output/BuildingsAreOutput/history/states");

   const hoi4::State state_one(1,
       {.provinces = {1, 4, 9, 16}, .civilian_factories = 2, .military_factories = 4, .dockyards = 6});

   OutputState("BuildingsAreOutput", state_one);

   ASSERT_TRUE(commonItems::DoesFileExist("output/BuildingsAreOutput/history/states/1.txt"));
   std::ifstream state_file("output/BuildingsAreOutput/history/states/1.txt");
   ASSERT_TRUE(state_file.is_open());
   std::stringstream state_file_stream;
   std::copy(std::istreambuf_iterator<char>(state_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(state_file_stream));
   state_file.close();
   EXPECT_THAT(state_file_stream.str(),
       testing::HasSubstr("\thistory = {\n"
                          "\t\tbuildings = {\n"
                          "\t\t\tindustrial_complex = 2\n"
                          "\t\t\tarms_factory = 4\n"
                          "\t\t\tdockyard = 6\n"
                          "\t\t}\n"
                          "\t}\n"));
}


TEST(Outhoi4StatesState, DockyardsNotOutputWhenZero)
{
   commonItems::TryCreateFolder("output");
   commonItems::TryCreateFolder("output/DockyardsNotOutputWhenZero");
   commonItems::TryCreateFolder("output/DockyardsNotOutputWhenZero/history");
   commonItems::TryCreateFolder("output/DockyardsNotOutputWhenZero/history/states");

   const hoi4::State state_one(1,
       {.provinces = {1, 4, 9, 16}, .civilian_factories = 2, .military_factories = 4, .dockyards = 0});

   OutputState("DockyardsNotOutputWhenZero", state_one);

   ASSERT_TRUE(commonItems::DoesFileExist("output/DockyardsNotOutputWhenZero/history/states/1.txt"));
   std::ifstream state_file("output/DockyardsNotOutputWhenZero/history/states/1.txt");
   ASSERT_TRUE(state_file.is_open());
   std::stringstream state_file_stream;
   std::copy(std::istreambuf_iterator<char>(state_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(state_file_stream));
   state_file.close();
   EXPECT_THAT(state_file_stream.str(),
       testing::HasSubstr("\thistory = {\n"
                          "\t\tbuildings = {\n"
                          "\t\t\tindustrial_complex = 2\n"
                          "\t\t\tarms_factory = 4\n"
                          "\t\t}\n"
                          "\t}\n"));
}


TEST(Outhoi4StatesState, NavalBasesCanBeOutput)
{
   commonItems::TryCreateFolder("output");
   commonItems::TryCreateFolder("output/NavalBasesCanBeOutput");
   commonItems::TryCreateFolder("output/NavalBasesCanBeOutput/history");
   commonItems::TryCreateFolder("output/NavalBasesCanBeOutput/history/states");

   const hoi4::State state_one(1, {.provinces = {1, 4, 9, 16}, .naval_base_location = 9, .naval_base_level = 3});

   OutputState("NavalBasesCanBeOutput", state_one);

   ASSERT_TRUE(commonItems::DoesFileExist("output/NavalBasesCanBeOutput/history/states/1.txt"));
   std::ifstream state_file("output/NavalBasesCanBeOutput/history/states/1.txt");
   ASSERT_TRUE(state_file.is_open());
   std::stringstream state_file_stream;
   std::copy(std::istreambuf_iterator<char>(state_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(state_file_stream));
   state_file.close();
   EXPECT_THAT(state_file_stream.str(),
       testing::HasSubstr("\thistory = {\n"
                          "\t\tbuildings = {\n"
                          "\t\t\tindustrial_complex = 0\n"
                          "\t\t\tarms_factory = 0\n"
                          "\t\t\t9 = {\n"
                          "\t\t\t\tnaval_base = 3\n"
                          "\t\t\t}\n"
                          "\t\t}\n"
                          "\t}\n"));
}


TEST(Outhoi4StatesState, NavalBasesAreNotOutputWhenLevelIsMissing)
{
   commonItems::TryCreateFolder("output");
   commonItems::TryCreateFolder("output/NavalBasesAreNotOutputWhenLevelIsMissing");
   commonItems::TryCreateFolder("output/NavalBasesAreNotOutputWhenLevelIsMissing/history");
   commonItems::TryCreateFolder("output/NavalBasesAreNotOutputWhenLevelIsMissing/history/states");

   const hoi4::State state_one(1, {.provinces = {1, 4, 9, 16}, .naval_base_location = 9});

   OutputState("NavalBasesAreNotOutputWhenLevelIsMissing", state_one);

   ASSERT_TRUE(commonItems::DoesFileExist("output/NavalBasesAreNotOutputWhenLevelIsMissing/history/states/1.txt"));
   std::ifstream state_file("output/NavalBasesAreNotOutputWhenLevelIsMissing/history/states/1.txt");
   ASSERT_TRUE(state_file.is_open());
   std::stringstream state_file_stream;
   std::copy(std::istreambuf_iterator<char>(state_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(state_file_stream));
   state_file.close();
   EXPECT_THAT(state_file_stream.str(),
       testing::HasSubstr("\thistory = {\n"
                          "\t\tbuildings = {\n"
                          "\t\t\tindustrial_complex = 0\n"
                          "\t\t\tarms_factory = 0\n"
                          "\t\t}\n"
                          "\t}\n"));
}


TEST(Outhoi4StatesState, NavalBasesAreNotOutputWhenLocationIsMissing)
{
   commonItems::TryCreateFolder("output");
   commonItems::TryCreateFolder("output/NavalBasesAreNotOutputWhenLocationIsMissing");
   commonItems::TryCreateFolder("output/NavalBasesAreNotOutputWhenLocationIsMissing/history");
   commonItems::TryCreateFolder("output/NavalBasesAreNotOutputWhenLocationIsMissing/history/states");

   const hoi4::State state_one(1, {.provinces = {1, 4, 9, 16}, .naval_base_level = 3});

   OutputState("NavalBasesAreNotOutputWhenLocationIsMissing", state_one);

   ASSERT_TRUE(commonItems::DoesFileExist("output/NavalBasesAreNotOutputWhenLocationIsMissing/history/states/1.txt"));
   std::ifstream state_file("output/NavalBasesAreNotOutputWhenLocationIsMissing/history/states/1.txt");
   ASSERT_TRUE(state_file.is_open());
   std::stringstream state_file_stream;
   std::copy(std::istreambuf_iterator<char>(state_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(state_file_stream));
   state_file.close();
   EXPECT_THAT(state_file_stream.str(),
       testing::HasSubstr("\thistory = {\n"
                          "\t\tbuildings = {\n"
                          "\t\t\tindustrial_complex = 0\n"
                          "\t\t\tarms_factory = 0\n"
                          "\t\t}\n"
                          "\t}\n"));
}

}  // namespace out