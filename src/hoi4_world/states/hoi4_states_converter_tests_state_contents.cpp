#include <ostream>
#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "external/fmt/include/fmt/format.h"
#include "src/configuration/configuration.h"
#include "src/hoi4_world/states/hoi4_state.h"
#include "src/hoi4_world/states/hoi4_states_converter.h"
#include "src/hoi4_world/world/hoi4_world_framework_builder.h"
#include "src/mappers/country/country_mapper.h"
#include "src/mappers/provinces/province_mapper.h"
#include "src/mappers/provinces/province_mapping_types.h"
#include "src/mappers/world/world_mapper_builder.h"
#include "src/maps/map_data.h"
#include "src/vic3_world/states/vic3_state.h"



// tests covering the conversion of the contents of states (industry, manpower, etc.)

namespace hoi4
{

// functions to allow test comparators to provide readable results
void PrintTo(const State& state, std::ostream* os)
{
   *os << "{\n";
   *os << fmt::format("\tid = {}\n", state.GetId());
   if (const auto& owner = state.GetOwner(); owner)
   {
      *os << fmt::format("\towner = {}\n", *owner);
   }
   *os << "\tprovinces {\n";
   for (const int province: state.GetProvinces())
   {
      *os << fmt::format("\t\t{}\n", province);
   }
   *os << "\t}\n";
   if (const int manpower = state.GetManpower(); manpower != 0)
   {
      *os << fmt::format("\tmanpower = {}\n", manpower);
   }
   if (const auto& resources = state.GetResources(); !resources.empty())
   {
      *os << "\tresources {\n";
      for (const auto& [resource, amount]: resources)
      {
         *os << fmt::format("\t\t{}: {}\n", resource, amount);
      }
      *os << "\t}\n";
   }
   *os << fmt::format("\tcategory = {}\n", state.GetCategory());
   *os << "\tvictory points {\n";
   for (const auto& [province, amount]: state.GetVictoryPoints())
   {
      *os << fmt::format("\t\t{} {}\n", province, amount);
   }
   *os << "\t}\n";
   if (const int civilian_factories = state.GetCivilianFactories(); civilian_factories != 0)
   {
      *os << fmt::format("\tcivilian factories = {}\n", civilian_factories);
   }
   if (const int military_factories = state.GetMilitaryFactories(); military_factories != 0)
   {
      *os << fmt::format("\tmilitary factories = {}\n", military_factories);
   }
   if (const int dockyards = state.GetDockyards(); dockyards != 0)
   {
      *os << fmt::format("\tdockyards = {}\n", dockyards);
   }
   if (const auto& naval_base_location = state.GetNavalBaseLocation(); naval_base_location)
   {
      *os << fmt::format("\tnaval base location = {}\n", *naval_base_location);
   }
   if (const auto& naval_base_level = state.GetNavalBaseLevel(); naval_base_level)
   {
      *os << fmt::format("\tnaval base level = {}\n", *naval_base_level);
   }
   *os << fmt::format("\tair base level = {}\n", state.GetAirBaseLevel());
   if (const auto& cores = state.GetCores(); !cores.empty())
   {
      *os << "\tcores {\n";
      for (const auto& core: cores)
      {
         *os << fmt::format("\t\t{}\n", core);
      }
      *os << "\t}\n";
   }
   *os << fmt::format("\tvic3 infrastructure = {}\n", state.GetVic3Infrastructure());
   *os << fmt::format("\tinfrastructure = {}\n", state.GetInfrastructure());
   *os << "}";
}


TEST(Hoi4worldStatesHoi4statesconverter, ManpowerIsConverted)
{
   vic3::WorldBuilder world = vic3::WorldBuilder::CreateNullWorld().AddStates({
       {1, vic3::State({.provinces = {1, 2, 3}, .population = 12345})},
       {2, vic3::State({.owner_number = 42, .provinces = {4, 5, 6}, .population = 67890})},
   });
   mappers::WorldMapperBuilder world_mapper =
       std::move(mappers::WorldMapperBuilder::CreateNullMapper().AddTestProvinces(6));
   world_mapper.CopyToVicWorld(world);
   hoi4::WorldFrameworkBuilder world_framework =
       WorldFrameworkBuilder::CreateNullWorldFramework().AddTestLandProvinces(6);
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = world_framework.CopyProvinceDefinitions(),
   });

   const auto hoi4_states = ConvertStates(world.Build(), world_mapper.Build(), world_framework.Build(), {}, map_data);

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1, {.provinces = {10, 20, 30}, .manpower = 12345}),
           State(2, {.provinces = {40, 50, 60}, .manpower = 67890})));
}


TEST(Hoi4worldStatesHoi4statesconverter, ManpowerInSplitStatesIsProportionalToTotalProvinces)
{
   vic3::WorldBuilder world = vic3::WorldBuilder::CreateNullWorld().AddStates({
       {1, vic3::State({.provinces = {1, 2, 3}, .population = 12345})},
       {2, vic3::State({.provinces = {4, 5, 6}, .population = 67890})},
   });
   mappers::WorldMapperBuilder world_mapper =
       std::move(mappers::WorldMapperBuilder::CreateNullMapper().AddTestProvinces(6));
   world_mapper.CopyToVicWorld(world);
   hoi4::WorldFrameworkBuilder world_framework =
       WorldFrameworkBuilder::CreateNullWorldFramework().AddTestLandProvinces(6);
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20"}},
               {"40", {"50"}},
           },
       .province_definitions = world_framework.CopyProvinceDefinitions(),
   });
   const auto hoi4_states = ConvertStates(world.Build(), world_mapper.Build(), world_framework.Build(), {}, map_data);

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1, {.provinces = {10, 20}, .manpower = 8230}),
           State(2, {.provinces = {30}, .manpower = 4115}),
           State(3, {.provinces = {40, 50}, .manpower = 45'260}),
           State(4, {.provinces = {60}, .manpower = 22'630})));
}

TEST(Hoi4worldStatesHoi4statesconverter, ManpowerInWastelandStatesIsProportionalToTotalProvinces)
{
   vic3::WorldBuilder world =
       vic3::WorldBuilder::CreateNullWorld()
           .AddStates({{1, vic3::State({.provinces = {1, 2, 3}, .population = 12345})},
               {2, vic3::State({.owner_number = 42, .provinces = {4, 5, 6}, .population = 67890})}})
           .AddTestStateRegions({{1, 2, 3}, {4, 5, 6}});
   mappers::WorldMapperBuilder world_mapper =
       std::move(mappers::WorldMapperBuilder::CreateNullMapper().AddTestProvinces(6));
   world_mapper.CopyToVicWorld(world);
   hoi4::WorldFrameworkBuilder world_framework =
       WorldFrameworkBuilder::CreateNullWorldFramework()
           .AddDefaultStates({
               {1, DefaultState({.provinces = {10, 20}})},
               {2, DefaultState({.impassable = true, .provinces = {30, 40, 50}})},
               {3, DefaultState({.provinces = {60}})},
           })
           .AddTestLandProvinces(6);
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = world_framework.CopyProvinceDefinitions(),
   });
   const auto hoi4_states = ConvertStates(world.Build(), world_mapper.Build(), world_framework.Build(), {}, map_data);

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1, {.provinces = {10, 20}, .manpower = 8230}),
           State(2, {.provinces = {30}, .manpower = 4115, .category = "wasteland"}),
           State(3, {.provinces = {60}, .manpower = 22'630}),
           State(4, {.provinces = {40, 50}, .manpower = 45'260, .category = "wasteland"})));
}


TEST(Hoi4worldStatesHoi4statesconverter, ManpowerIsLogged)
{
   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   vic3::WorldBuilder world =
       vic3::WorldBuilder::CreateNullWorld()
           .AddStates({{1, vic3::State({.owner_tag = "ONE", .provinces = {1, 2, 3}, .population = 300'000})},
               {2, vic3::State({.owner_tag = "TWO", .provinces = {4, 5, 6}, .population = 20'000})}})
           .AddTestStateRegions({{1, 2, 3}, {4, 5, 6}});
   mappers::WorldMapperBuilder world_mapper = std::move(
       mappers::WorldMapperBuilder::CreateNullMapper().AddTestProvinces(6).AddCountries({{1, "ONE"}, {2, "TWO"}}));
   world_mapper.CopyToVicWorld(world);
   hoi4::WorldFrameworkBuilder world_framework = WorldFrameworkBuilder::CreateNullWorldFramework()
                                                     .AddDefaultStates({{1, DefaultState({.manpower = 100'000})},
                                                         {2, DefaultState({.manpower = 20'000})},
                                                         {3, DefaultState({.manpower = 3'000})}})
                                                     .AddTestLandProvinces(6);
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = world_framework.CopyProvinceDefinitions(),
   });
   const auto hoi4_states = ConvertStates(world.Build(), world_mapper.Build(), world_framework.Build(), {}, map_data);

   std::cout.rdbuf(cout_buffer);

   EXPECT_THAT(log.str(),
       testing::HasSubstr("[INFO] \t\tManpower conversion: total=320000.000, target=123000.000, diff=160.163%"));
}


TEST(Hoi4worldStatesHoi4statesconverter, IndustryIsConverted)
{
   vic3::WorldBuilder world = vic3::WorldBuilder::CreateNullWorld()
                                  .AddStates({{1, vic3::State({.owner_number = 1, .provinces = {1, 2, 3}})},
                                      {2, vic3::State({.owner_number = 2, .provinces = {4, 5, 6}})}})
                                  .AddTestStateRegions({{1, 2, 3}, {4, 5, 6}})
                                  .AddBuildings({vic3::Building("", 1, 875'000), vic3::Building("", 2, 875'000)});
   mappers::WorldMapperBuilder world_mapper = std::move(
       mappers::WorldMapperBuilder::CreateNullMapper().AddTestProvinces(6).AddCountries({{1, "ONE"}, {2, "TWO"}}));
   world_mapper.CopyToVicWorld(world);
   hoi4::WorldFrameworkBuilder world_framework =
       WorldFrameworkBuilder::CreateNullWorldFramework().AddTestLandProvinces(6);
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = world_framework.CopyProvinceDefinitions(),
   });
   const auto hoi4_states = ConvertStates(world.Build(), world_mapper.Build(), world_framework.Build(), {}, map_data);

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1,
                                {.owner = "ONE",
                                    .provinces = {10, 20, 30},
                                    .civilian_factories = 3,
                                    .military_factories = 2,
                                    .air_base_level = 1}),
           State(2,
               {.owner = "TWO",
                   .provinces = {40, 50, 60},
                   .civilian_factories = 3,
                   .military_factories = 2,
                   .air_base_level = 1})));
}


TEST(Hoi4worldStatesHoi4statesconverter, DockyardsAreConvertedInCoastalStates)
{
   vic3::WorldBuilder world = vic3::WorldBuilder::CreateNullWorld()
                                  .AddStates({
                                      {1, vic3::State({.owner_number = 1, .provinces = {1, 2, 3}})},
                                      {2, vic3::State({.owner_number = 2, .provinces = {4, 5, 6}})},
                                  })
                                  .AddBuildings({vic3::Building("", 1, 875'000), vic3::Building("", 2, 875'000)});
   mappers::WorldMapperBuilder world_mapper = std::move(
       mappers::WorldMapperBuilder::CreateNullMapper().AddTestProvinces(6).AddCountries({{1, "ONE"}, {2, "TWO"}}));
   world_mapper.CopyToVicWorld(world);
   hoi4::WorldFrameworkBuilder world_framework =
       WorldFrameworkBuilder::CreateNullWorldFramework().AddTestLandProvinces(6).AddCoastalProvinces(
           std::map<int, std::vector<int>>{{40, {41}}});
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = world_framework.CopyProvinceDefinitions(),
   });
   const auto hoi4_states = ConvertStates(world.Build(), world_mapper.Build(), world_framework.Build(), {}, map_data);

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1,
                                StateOptions{.owner = "ONE",
                                    .provinces = {10, 20, 30},
                                    .civilian_factories = 3,
                                    .military_factories = 2,
                                    .air_base_level = 1}),
           State(2,
               StateOptions{.owner = "TWO",
                   .provinces = {40, 50, 60},
                   .civilian_factories = 2,
                   .military_factories = 2,
                   .dockyards = 1,
                   .air_base_level = 1})));
}


TEST(Hoi4worldStatesHoi4statesconverter, IndustryIsLogged)
{
   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   vic3::WorldBuilder world = vic3::WorldBuilder::CreateNullWorld()
                                  .AddStates({{1, vic3::State({.owner_number = 1, .provinces = {1, 2, 3}})},
                                      {2, vic3::State({.owner_number = 2, .provinces = {4, 5, 6}})}})
                                  .AddTestStateRegions({{1, 2, 3}, {4, 5, 6}})
                                  .AddBuildings({vic3::Building("", 1, 875'000), vic3::Building("", 2, 875'000)});
   mappers::WorldMapperBuilder world_mapper = std::move(
       mappers::WorldMapperBuilder::CreateNullMapper().AddTestProvinces(6).AddCountries({{1, "ONE"}, {2, "TWO"}}));
   world_mapper.CopyToVicWorld(world);
   hoi4::WorldFrameworkBuilder world_framework = WorldFrameworkBuilder::CreateNullWorldFramework()
                                                     .AddDefaultStates({
                                                         {1, DefaultState({.civilian_factories = 1})},
                                                         {2, DefaultState({.military_factories = 2})},
                                                         {3, DefaultState({.dockyards = 3})},
                                                     })
                                                     .AddTestLandProvinces(6);
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = world_framework.CopyProvinceDefinitions(),
   });
   const auto hoi4_states = ConvertStates(world.Build(), world_mapper.Build(), world_framework.Build(), {}, map_data);

   std::cout.rdbuf(cout_buffer);

   EXPECT_THAT(log.str(),
       testing::HasSubstr("[INFO] \t\tTotal factories conversion: total=10.000, target=6.000, diff=66.667%"));
   EXPECT_THAT(log.str(),
       testing::HasSubstr("[INFO] \t\tCivilian factories conversion: total=6.000, target=1.000, diff=500.000%"));
   EXPECT_THAT(log.str(),
       testing::HasSubstr("[INFO] \t\tMilitary factories conversion: total=4.000, target=2.000, diff=100.000%"));
   EXPECT_THAT(log.str(),
       testing::HasSubstr("[INFO] \t\tDockyards conversion: total=0.000, target=3.000, diff=-100.000%"));
}


TEST(Hoi4worldStatesHoi4statesconverter, IndustryIsNotConvertedInUnownedStates)
{
   vic3::WorldBuilder world =
       vic3::WorldBuilder::CreateNullWorld()
           .AddStates({{1, vic3::State({.provinces = {1, 2, 3}})}, {2, vic3::State({.provinces = {4, 5, 6}})}})
           .AddTestStateRegions({{1, 2, 3}, {4, 5, 6}})
           .AddBuildings({vic3::Building("", 1, 1'250'000), vic3::Building("", 2, 1'250'000)});
   mappers::WorldMapperBuilder world_mapper = std::move(
       mappers::WorldMapperBuilder::CreateNullMapper().AddTestProvinces(6).AddCountries({{1, "ONE"}, {2, "TWO"}}));
   world_mapper.CopyToVicWorld(world);
   hoi4::WorldFrameworkBuilder world_framework =
       WorldFrameworkBuilder::CreateNullWorldFramework().AddTestLandProvinces(6);
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = world_framework.CopyProvinceDefinitions(),
   });
   const auto hoi4_states = ConvertStates(world.Build(), world_mapper.Build(), world_framework.Build(), {}, map_data);

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(
           State(1, {.provinces = {10, 20, 30}, .civilian_factories = 0, .military_factories = 0, .air_base_level = 0}),
           State(2,
               {.provinces = {40, 50, 60}, .civilian_factories = 0, .military_factories = 0, .air_base_level = 0})));
}


TEST(Hoi4worldStatesHoi4statesconverter, IndustryIsCappedAtTwelve)
{
   vic3::WorldBuilder world = vic3::WorldBuilder::CreateNullWorld()
                                  .AddTestStates({{1, 2, 3}})
                                  .AddTestStateRegions({{1, 2, 3}, {4, 5, 6}})
                                  .AddBuildings({
                                      vic3::Building("", 1, 2'100'000),
                                  });
   mappers::WorldMapperBuilder world_mapper =
       std::move(mappers::WorldMapperBuilder::CreateNullMapper().AddTestProvinces(6).AddCountries({{1, "ONE"}}));
   world_mapper.CopyToVicWorld(world);
   hoi4::WorldFrameworkBuilder world_framework =
       WorldFrameworkBuilder::CreateNullWorldFramework().AddTestLandProvinces(6);
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = world_framework.CopyProvinceDefinitions(),
   });
   const auto hoi4_states = ConvertStates(world.Build(), world_mapper.Build(), world_framework.Build(), {}, map_data);

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1,
           {.owner = "ONE",
               .provinces = {10, 20, 30},
               .civilian_factories = 8,
               .military_factories = 4,
               .air_base_level = 3})));
}

TEST(Hoi4worldStatesHoi4statesconverter, StatesAreSortedByIndustry)
{
   vic3::WorldBuilder world =
       vic3::WorldBuilder::CreateNullWorld()
           .AddTestStates({{1, 2, 3}, {4, 5}})
           .AddStates({{3, vic3::State({.owner_number = 2, .provinces = {6}})}})
           .AddTestStateRegions({{1, 2, 3}, {4, 5, 6}})
           .AddBuildings(
               {vic3::Building("", 1, 350'000), vic3::Building("", 2, 2'100'000), vic3::Building("", 3, 1'575'000)});
   mappers::WorldMapperBuilder world_mapper = std::move(
       mappers::WorldMapperBuilder::CreateNullMapper().AddTestProvinces(6).AddCountries({{1, "ONE"}, {2, "TWO"}}));
   world_mapper.CopyToVicWorld(world);
   hoi4::WorldFrameworkBuilder world_framework =
       WorldFrameworkBuilder::CreateNullWorldFramework().AddTestLandProvinces(6);
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = world_framework.CopyProvinceDefinitions(),
   });
   const auto hoi4_states = ConvertStates(world.Build(), world_mapper.Build(), world_framework.Build(), {}, map_data);

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1,
                                {.owner = "TWO",
                                    .provinces = {40, 50},
                                    .civilian_factories = 8,
                                    .military_factories = 4,
                                    .air_base_level = 3}),
           State(2,
               {.owner = "TWO",
                   .provinces = {60},
                   .civilian_factories = 6,
                   .military_factories = 3,
                   .air_base_level = 2}),
           State(3,
               {.owner = "ONE",
                   .provinces = {10, 20, 30},
                   .civilian_factories = 1,
                   .military_factories = 1,
                   .air_base_level = 0})));
}


TEST(Hoi4worldStatesHoi4statesconverter, UnconvertedIndustryIsConvertedInNextStateOfSameOwner)
{
   vic3::WorldBuilder world =
       vic3::WorldBuilder::CreateNullWorld()
           .AddStates({
               {1, vic3::State({.owner_number = 1, .provinces = {1, 2}})},
               {2, vic3::State({.owner_number = 1, .provinces = {3, 4}})},
               {3, vic3::State({.owner_number = 1, .provinces = {5, 6}})},
           })
           .AddTestStateRegions({{1, 2, 3}, {4, 5, 6}})
           .AddBuildings({vic3::Building("", 1, 5'250'000), vic3::Building("", 2, 0), vic3::Building("", 3, 0)});
   mappers::WorldMapperBuilder world_mapper =
       std::move(mappers::WorldMapperBuilder::CreateNullMapper().AddTestProvinces(6).AddCountries({{1, "ONE"}}));
   world_mapper.CopyToVicWorld(world);
   hoi4::WorldFrameworkBuilder world_framework =
       WorldFrameworkBuilder::CreateNullWorldFramework().AddTestLandProvinces(6);
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20"}},
               {"30", {"40"}},
               {"50", {"60"}},
           },
       .province_definitions = world_framework.CopyProvinceDefinitions(),
   });
   const auto hoi4_states = ConvertStates(world.Build(), world_mapper.Build(), world_framework.Build(), {}, map_data);

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1,
                                {.owner = "ONE",
                                    .provinces = {10, 20},
                                    .civilian_factories = 3,
                                    .military_factories = 9,
                                    .air_base_level = 3}),
           State(2,
               {.owner = "ONE",
                   .provinces = {30, 40},
                   .civilian_factories = 12,
                   .military_factories = 0,
                   .air_base_level = 3}),
           State(3,
               {.owner = "ONE",
                   .provinces = {50, 60},
                   .civilian_factories = 6,
                   .military_factories = 0,
                   .air_base_level = 1})));
}


TEST(Hoi4worldStatesHoi4statesconverter, IndustryInSplitStatesIsProportionalToTotalProvinces)
{
   vic3::WorldBuilder world = vic3::WorldBuilder::CreateNullWorld()
                                  .AddTestStates({{1, 2, 3}, {4, 5, 6}})
                                  .AddTestStateRegions({{1, 2, 3}, {4, 5, 6}})
                                  .AddBuildings({vic3::Building("", 1, 700'000), vic3::Building("", 2, 1'575'000)});
   mappers::WorldMapperBuilder world_mapper = std::move(
       mappers::WorldMapperBuilder::CreateNullMapper().AddTestProvinces(6).AddCountries({{1, "ONE"}, {2, "TWO"}}));
   world_mapper.CopyToVicWorld(world);
   hoi4::WorldFrameworkBuilder world_framework =
       WorldFrameworkBuilder::CreateNullWorldFramework().AddTestLandProvinces(6);
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20"}},
               {"40", {"50"}},
           },
       .province_definitions = world_framework.CopyProvinceDefinitions(),
   });
   const auto hoi4_states = ConvertStates(world.Build(), world_mapper.Build(), world_framework.Build(), {}, map_data);

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1,
                                {.owner = "TWO",
                                    .provinces = {40, 50},
                                    .civilian_factories = 4,
                                    .military_factories = 2,
                                    .air_base_level = 1}),
           State(2,
               {.owner = "TWO",
                   .provinces = {60},
                   .civilian_factories = 2,
                   .military_factories = 1,
                   .air_base_level = 0}),
           State(3,
               {.owner = "ONE",
                   .provinces = {10, 20},
                   .civilian_factories = 2,
                   .military_factories = 1,
                   .air_base_level = 0}),
           State(4,
               {.owner = "ONE",
                   .provinces = {30},
                   .civilian_factories = 0,
                   .military_factories = 1,
                   .air_base_level = 0})));
}


TEST(Hoi4worldStatesHoi4statesconverter, IndustryInWastelandSplitStatesIsZero)
{
   vic3::WorldBuilder world = vic3::WorldBuilder::CreateNullWorld()
                                  .AddTestStates({{1, 2, 3}, {4, 5, 6}})
                                  .AddTestStateRegions({{1, 2, 3}, {4, 5, 6}})
                                  .AddBuildings({vic3::Building("", 1, 875'000), vic3::Building("", 2, 875'000)});
   mappers::WorldMapperBuilder world_mapper = std::move(
       mappers::WorldMapperBuilder::CreateNullMapper().AddTestProvinces(6).AddCountries({{1, "ONE"}, {2, "TWO"}}));
   world_mapper.CopyToVicWorld(world);
   hoi4::WorldFrameworkBuilder world_framework =
       WorldFrameworkBuilder::CreateNullWorldFramework()
           .AddDefaultStates({
               {1, DefaultState({.provinces = {10, 20}})},
               {2, DefaultState({.impassable = true, .provinces = {30, 40, 50}})},
               {3, DefaultState({.provinces = {60}})},
           })
           .AddTestLandProvinces(6);
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = world_framework.CopyProvinceDefinitions(),
   });
   const auto hoi4_states = ConvertStates(world.Build(), world_mapper.Build(), world_framework.Build(), {}, map_data);

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1,
                                {.owner = "ONE",
                                    .provinces = {10, 20},
                                    .civilian_factories = 3,
                                    .military_factories = 2,
                                    .air_base_level = 1}),
           State(2, {.owner = "ONE", .provinces = {30}, .category = "wasteland", .air_base_level = 0}),
           State(3,
               {.owner = "TWO",
                   .provinces = {60},
                   .civilian_factories = 3,
                   .military_factories = 2,
                   .air_base_level = 1}),
           State(4, {.owner = "TWO", .provinces = {40, 50}, .category = "wasteland", .air_base_level = 0})));
}

TEST(Hoi4worldStatesHoi4statesconverter, NavalBaseDefaultsToZero)
{
   vic3::WorldBuilder world = vic3::WorldBuilder::CreateNullWorld()
                                  .AddTestStates({{1, 2, 3}})
                                  .AddTestStateRegions({{1, 2, 3}, {4, 5, 6}})
                                  .AddStateRegions({
                                      {"REGION_ONE", vic3::StateRegion({{"x000002", "port"}}, {})},
                                      {"REGION_TWO", vic3::StateRegion({{"x000005", "port"}}, {})},
                                  });
   mappers::WorldMapperBuilder world_mapper = std::move(
       mappers::WorldMapperBuilder::CreateNullMapper().AddTestProvinces(6).AddCountries({{1, "ONE"}, {2, "TWO"}}));
   world_mapper.CopyToVicWorld(world);
   hoi4::WorldFrameworkBuilder world_framework =
       WorldFrameworkBuilder::CreateNullWorldFramework().AddTestLandProvinces(6).AddCoastalProvinces(
           {{20, {21}}, {50, {51}}});
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = world_framework.CopyProvinceDefinitions(),
   });
   const auto hoi4_states = ConvertStates(world.Build(), world_mapper.Build(), world_framework.Build(), {}, map_data);

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1,
           {.owner = "ONE",
               .provinces = {10, 20, 30},
               .victory_points = {{20, 1}},
               .naval_base_location = std::nullopt,
               .naval_base_level = std::nullopt})));
}
TEST(Hoi4worldStatesHoi4statesconverter, NavalBasesAreConvertedInCoastalStates)
{
   vic3::WorldBuilder world = vic3::WorldBuilder::CreateNullWorld()
                                  .AddTestStates({{1, 2, 3}, {4, 5, 6}})
                                  .AddTestStateRegions({{1, 2, 3}, {4, 5, 6}})
                                  .AddStateRegions({
                                      {"REGION_ONE", vic3::StateRegion({{"x000002", "port"}}, {})},
                                      {"REGION_TWO", vic3::StateRegion({{"x000005", "port"}}, {})},
                                  })
                                  .AddBuildings({vic3::Building(vic3::BuildingType::NavalBase, 1, 0.0F, 9.0F, {}),
                                      vic3::Building(vic3::BuildingType::NavalBase, 2, 0.0F, 10.0F, {})});
   mappers::WorldMapperBuilder world_mapper = std::move(
       mappers::WorldMapperBuilder::CreateNullMapper().AddTestProvinces(6).AddCountries({{1, "ONE"}, {2, "TWO"}}));
   world_mapper.CopyToVicWorld(world);
   hoi4::WorldFrameworkBuilder world_framework =
       WorldFrameworkBuilder::CreateNullWorldFramework().AddTestLandProvinces(6).AddCoastalProvinces(
           {{20, {21}}, {50, {51}}});
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = world_framework.CopyProvinceDefinitions(),
   });
   const auto hoi4_states = ConvertStates(world.Build(), world_mapper.Build(), world_framework.Build(), {}, map_data);

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1,
                                {.owner = "ONE",
                                    .provinces = {10, 20, 30},
                                    .victory_points = {{20, 1}},
                                    .naval_base_location = 20,
                                    .naval_base_level = 1}),
           State(2,
               {.owner = "TWO",
                   .provinces = {40, 50, 60},
                   .victory_points = {{50, 1}},
                   .naval_base_location = 50,
                   .naval_base_level = 2})));
}

TEST(Hoi4worldStatesHoi4statesconverter, NavalBasesPrioritisePorts)
{
   vic3::WorldBuilder world = vic3::WorldBuilder::CreateNullWorld()
                                  .AddTestStates({{1, 2, 3}, {4, 5, 6}})
                                  .AddTestStateRegions({{1, 2, 3}, {4, 5, 6}})
                                  .AddStateRegions({
                                      {"REGION_ONE", vic3::StateRegion({{"x000002", "farm"}, {"x000001", "port"}}, {})},
                                      {"REGION_TWO", vic3::StateRegion({{"x000005", "mine"}, {"x000004", "city"}}, {})},
                                  })
                                  .AddBuildings({vic3::Building(vic3::BuildingType::NavalBase, 1, 0.0F, 9.0F, {}),
                                      vic3::Building(vic3::BuildingType::NavalBase, 2, 0.0F, 10.0F, {})});
   mappers::WorldMapperBuilder world_mapper = std::move(
       mappers::WorldMapperBuilder::CreateNullMapper().AddTestProvinces(6).AddCountries({{1, "ONE"}, {2, "TWO"}}));
   world_mapper.CopyToVicWorld(world);
   // Make all HoI provinces coastal.
   hoi4::WorldFrameworkBuilder world_framework =
       WorldFrameworkBuilder::CreateNullWorldFramework().AddTestLandProvinces(6).AddCoastalProvinces(
           {{10, {11}}, {20, {21}}, {30, {31}}, {40, {41}}, {50, {51}}});
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = world_framework.CopyProvinceDefinitions(),
   });
   const auto hoi4_states = ConvertStates(world.Build(), world_mapper.Build(), world_framework.Build(), {}, map_data);

   // Naval base locations are the 'port' and 'city' provinces.
   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1,
                                {.owner = "ONE",
                                    .provinces = {10, 20, 30},
                                    .victory_points = {{10, 1}},
                                    .naval_base_location = 10,
                                    .naval_base_level = 1}),
           State(2,
               {.owner = "TWO",
                   .provinces = {40, 50, 60},
                   .victory_points = {{40, 1}},
                   .naval_base_location = 40,
                   .naval_base_level = 2})));
}

TEST(Hoi4worldStatesHoi4statesconverter, AirBaseLevelDefaultsToZero)
{
   vic3::WorldBuilder world =
       vic3::WorldBuilder::CreateNullWorld()
           .AddStates({{1, vic3::State({.provinces = {1, 2, 3}})}, {2, vic3::State({.provinces = {4, 5, 6}})}})
           .AddTestStateRegions({{1, 2, 3}, {4, 5, 6}});
   mappers::WorldMapperBuilder world_mapper =
       std::move(mappers::WorldMapperBuilder::CreateNullMapper().AddTestProvinces(6));
   world_mapper.CopyToVicWorld(world);
   WorldFrameworkBuilder world_framework = WorldFrameworkBuilder::CreateNullWorldFramework().AddTestLandProvinces(6);
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = world_framework.CopyProvinceDefinitions(),
   });
   const auto hoi4_states = ConvertStates(world.Build(), world_mapper.Build(), world_framework.Build(), {}, map_data);

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1, {.provinces = {10, 20, 30}, .air_base_level = 0}),
           State(2, {.provinces = {40, 50, 60}, .air_base_level = 0})));
}


TEST(Hoi4worldStatesHoi4statesconverter, AirBaseLevelIncreasesFromIndustry)
{
   vic3::WorldBuilder world = vic3::WorldBuilder::CreateNullWorld()
                                  .AddStates({{1, vic3::State({.owner_number = 1, .provinces = {1, 2, 3}})},
                                      {2, vic3::State({.owner_number = 2, .provinces = {4, 5, 6}})}})
                                  .AddTestStateRegions({{1, 2, 3}, {4, 5, 6}})
                                  .AddBuildings({vic3::Building("", 1, 700'000), vic3::Building("", 2, 1'400'000)});
   mappers::WorldMapperBuilder world_mapper = std::move(
       mappers::WorldMapperBuilder::CreateNullMapper().AddTestProvinces(6).AddCountries({{1, "ONE"}, {2, "TWO"}}));
   world_mapper.CopyToVicWorld(world);
   WorldFrameworkBuilder world_framework = WorldFrameworkBuilder::CreateNullWorldFramework().AddTestLandProvinces(6);
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = world_framework.CopyProvinceDefinitions(),
   });
   const auto hoi4_states = ConvertStates(world.Build(), world_mapper.Build(), world_framework.Build(), {}, map_data);

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1,
                                {.owner = "TWO",
                                    .provinces = {40, 50, 60},
                                    .civilian_factories = 5,
                                    .military_factories = 3,
                                    .air_base_level = 2}),
           State(2,
               {.owner = "ONE",
                   .provinces = {10, 20, 30},
                   .civilian_factories = 2,
                   .military_factories = 2,
                   .air_base_level = 1})));
}


TEST(Hoi4worldStatesHoi4statesconverter, AirBaseLevelIncreasesFromInfrastructure)
{
   vic3::WorldBuilder world =
       vic3::WorldBuilder::CreateNullWorld()
           .AddStates({{1, vic3::State({.owner_number = 1, .infrastructure = 123.4F, .provinces = {1, 2, 3}})},
               {2, vic3::State({.owner_number = 2, .infrastructure = 567.8F, .provinces = {4, 5, 6}})}})
           .AddTestStateRegions({{1, 2, 3}, {4, 5, 6}});
   mappers::WorldMapperBuilder world_mapper = std::move(
       mappers::WorldMapperBuilder::CreateNullMapper().AddTestProvinces(6).AddCountries({{1, "ONE"}, {2, "TWO"}}));
   world_mapper.CopyToVicWorld(world);
   WorldFrameworkBuilder world_framework = WorldFrameworkBuilder::CreateNullWorldFramework().AddTestLandProvinces(6);
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = world_framework.CopyProvinceDefinitions(),
   });
   const auto hoi4_states = ConvertStates(world.Build(), world_mapper.Build(), world_framework.Build(), {}, map_data);

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1,
                                {.owner = "ONE",
                                    .provinces = {10, 20, 30},
                                    .air_base_level = 0,
                                    .vic3_infrastructure = 123.4F,
                                    .infrastructure = 1}),
           State(2,
               {.owner = "TWO",
                   .provinces = {40, 50, 60},
                   .air_base_level = 1,
                   .vic3_infrastructure = 567.8F,
                   .infrastructure = 3})));
}


TEST(Hoi4worldStatesHoi4statesconverter, ResourcesDefaultToEmpty)
{
   vic3::WorldBuilder world =
       vic3::WorldBuilder::CreateNullWorld()
           .AddStates({{1, vic3::State({.provinces = {1, 2, 3}})}, {2, vic3::State({.provinces = {4, 5, 6}})}})
           .AddTestStateRegions({{1, 2, 3}, {4, 5, 6}});
   mappers::WorldMapperBuilder world_mapper =
       std::move(mappers::WorldMapperBuilder::CreateNullMapper().AddTestProvinces(6));
   world_mapper.CopyToVicWorld(world);
   hoi4::WorldFrameworkBuilder world_framework =
       WorldFrameworkBuilder::CreateNullWorldFramework().AddTestLandProvinces(6);
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = world_framework.CopyProvinceDefinitions(),
   });
   const auto hoi4_states = ConvertStates(world.Build(), world_mapper.Build(), world_framework.Build(), {}, map_data);

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1, {.provinces = {10, 20, 30}, .resources = {}}),
           State(2, {.provinces = {40, 50, 60}, .resources = {}})));
}


TEST(Hoi4worldStatesHoi4statesconverter, ResourcesAreAssigned)
{
   vic3::WorldBuilder world = vic3::WorldBuilder::CreateNullWorld()
                                  .AddTestStates({{1, 2, 3}, {4, 5, 6}})
                                  .AddTestStateRegions({{1, 2, 3}, {4, 5, 6}});
   mappers::WorldMapperBuilder world_mapper =
       std::move(mappers::WorldMapperBuilder::CreateNullMapper().AddTestProvinces(6));
   world_mapper.CopyToVicWorld(world);
   hoi4::WorldFrameworkBuilder world_framework = WorldFrameworkBuilder::CreateNullWorldFramework()
                                                     .AddResources({
                                                         /* 10 not included to force the continue */
                                                         {
                                                             20,
                                                             {{"test_resource", 2.0}},
                                                         },
                                                         {
                                                             30,
                                                             {{"test_resource", 3.0}},
                                                         },
                                                         {40,
                                                             {
                                                                 {"test_resource", 7.0},
                                                                 {"test_resource_two", 11.0},
                                                             }},
                                                     })
                                                     .AddTestLandProvinces(6);
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = world_framework.CopyProvinceDefinitions(),
   });
   const auto hoi4_states = ConvertStates(world.Build(), world_mapper.Build(), world_framework.Build(), {}, map_data, {.dynamic_resources = false});

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1, {.provinces = {10, 20, 30}, .resources = {{"test_resource", 5.0}}}),
           State(2, {.provinces = {40, 50, 60}, .resources = {{"test_resource", 7.0}, {"test_resource_two", 11.0}}})));
}


TEST(Hoi4worldStatesHoi4statesconverter, ResourcesAreCalculated)
{
   vic3::WorldBuilder world = vic3::WorldBuilder::CreateNullWorld()
                                  .AddTestStates({{1, 2, 3}, {4, 5, 6}})
                                  .AddTestStateRegions({{1, 2, 3}, {4, 5, 6}})
                                  .AddBuildings({vic3::Building("building_iron_mine", 1, 1),
                                      vic3::Building("building_some_factory", 1, 1),
                                      vic3::Building("building_iron_mine", 2, 1),
                                      vic3::Building("building_oil_rig", 2, 1)});
   mappers::WorldMapperBuilder world_mapper =
       std::move(mappers::WorldMapperBuilder::CreateNullMapper().AddTestProvinces(6).LoadResourceMapper(
           "test_files/configurables/resource_mappings.txt"));
   world_mapper.CopyToVicWorld(world);
   hoi4::WorldFrameworkBuilder world_framework =
       WorldFrameworkBuilder::CreateNullWorldFramework().AddTestLandProvinces(6);
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = world_framework.CopyProvinceDefinitions(),
   });
   const auto hoi4_states = ConvertStates(world.Build(),
       world_mapper.Build(),
       world_framework.Build(),
       {},
       map_data,
       configuration::Configuration{.dynamic_resources = true});

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1, {.provinces = {10, 20, 30}, .resources = {{"steel", 500.0}}}),
           State(2, {.provinces = {40, 50, 60}, .resources = {{"steel", 500.0}, {"oil", 250.0}}})));
}


TEST(Hoi4worldStatesHoi4statesconverter, ResourcesAreLogged)
{
   vic3::WorldBuilder world = vic3::WorldBuilder::CreateNullWorld()
                                  .AddTestStates({{1, 2, 3}, {4, 5, 6}})
                                  .AddTestStateRegions({{1, 2, 3}, {4, 5, 6}});
   mappers::WorldMapperBuilder world_mapper =
       std::move(mappers::WorldMapperBuilder::CreateNullMapper().AddTestProvinces(6));
   world_mapper.CopyToVicWorld(world);
   hoi4::WorldFrameworkBuilder world_framework = WorldFrameworkBuilder::CreateNullWorldFramework()
                                                     .AddResources({
                                                         /* 10 not included to force the continue */
                                                         {
                                                             20,
                                                             {{"test_resource", 2.0}},
                                                         },
                                                         {
                                                             30,
                                                             {{"test_resource", 3.0}},
                                                         },
                                                         {40,
                                                             {
                                                                 {"test_resource", 7.0},
                                                                 {"test_resource_two", 11.0},
                                                             }},
                                                     })
                                                     .AddTestLandProvinces(6);
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = world_framework.CopyProvinceDefinitions(),
   });

   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const auto hoi4_states = ConvertStates(world.Build(), world_mapper.Build(), world_framework.Build(), {}, map_data, { .dynamic_resources = false });

   std::cout.rdbuf(cout_buffer);

   EXPECT_THAT(log.str(), testing::HasSubstr("[INFO] \t\t\tConverter Resource test_resource:12"));
   EXPECT_THAT(log.str(), testing::HasSubstr("[INFO] \t\t\tConverter Resource test_resource_two:11"));
}


TEST(Hoi4worldStatesHoi4statesconverter, CategoryDefaultsToRural)
{
   vic3::WorldBuilder world = vic3::WorldBuilder::CreateNullWorld()
                                  .AddTestStates({{1, 2, 3}, {4, 5, 6}})
                                  .AddTestStateRegions({{1, 2, 3}, {4, 5, 6}});
   mappers::WorldMapperBuilder world_mapper =
       std::move(mappers::WorldMapperBuilder::CreateNullMapper().AddTestProvinces(6));
   world_mapper.CopyToVicWorld(world);
   hoi4::WorldFrameworkBuilder world_framework =
       WorldFrameworkBuilder::CreateNullWorldFramework().AddTestLandProvinces(6);
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = world_framework.CopyProvinceDefinitions(),
   });
   const auto hoi4_states = ConvertStates(world.Build(), world_mapper.Build(), world_framework.Build(), {}, map_data);

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1, {.provinces = {10, 20, 30}, .category = "rural"}),
           State(2, {.provinces = {40, 50, 60}, .category = "rural"})));
}


TEST(Hoi4worldStatesHoi4statesconverter, CategoriesAreSet)
{
   vic3::WorldBuilder world = vic3::WorldBuilder::CreateNullWorld()
                                  .AddTestStates({{1, 2, 3}, {4, 5, 6}})
                                  .AddTestStateRegions({{1, 2, 3}, {4, 5, 6}})
                                  .AddBuildings({vic3::Building("", 1, 700'000), vic3::Building("", 2, 1'400'000)});
   mappers::WorldMapperBuilder world_mapper = std::move(
       mappers::WorldMapperBuilder::CreateNullMapper().AddTestProvinces(6).AddCountries({{1, "ONE"}, {2, "TWO"}}));
   world_mapper.CopyToVicWorld(world);
   hoi4::WorldFrameworkBuilder world_framework = WorldFrameworkBuilder::CreateNullWorldFramework()
                                                     .SetStateCategories(hoi4::StateCategories({
                                                         {5, "test_category_one"},
                                                         {7, "test_category_two"},
                                                         {9, "test_category_three"},
                                                         {11, "test_category_four"},
                                                     }))
                                                     .AddTestLandProvinces(6);
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = world_framework.CopyProvinceDefinitions(),
   });
   const auto hoi4_states = ConvertStates(world.Build(), world_mapper.Build(), world_framework.Build(), {}, map_data);

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1,
                                {.owner = "TWO",
                                    .provinces = {40, 50, 60},
                                    .category = "test_category_four",
                                    .civilian_factories = 5,
                                    .military_factories = 3,
                                    .air_base_level = 2}),
           State(2,
               {.owner = "ONE",
                   .provinces = {10, 20, 30},
                   .category = "test_category_two",
                   .civilian_factories = 2,
                   .military_factories = 2,
                   .air_base_level = 1})));
}


TEST(Hoi4worldStatesHoi4statesconverter, VictoryPointsDefaultToEmpty)
{
   vic3::WorldBuilder world = vic3::WorldBuilder::CreateNullWorld()
                                  .AddTestStates({{1, 2, 3}, {4, 5, 6}})
                                  .AddTestStateRegions({{1, 2, 3}, {4, 5, 6}});
   mappers::WorldMapperBuilder world_mapper =
       std::move(mappers::WorldMapperBuilder::CreateNullMapper().AddTestProvinces(6));
   world_mapper.CopyToVicWorld(world);
   hoi4::WorldFrameworkBuilder world_framework =
       WorldFrameworkBuilder::CreateNullWorldFramework().AddTestLandProvinces(6);
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = world_framework.CopyProvinceDefinitions(),
   });
   const auto hoi4_states = ConvertStates(world.Build(), world_mapper.Build(), world_framework.Build(), {}, map_data);

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1, {.provinces = {10, 20, 30}, .victory_points = {}}),
           State(2, {.provinces = {40, 50, 60}, .victory_points = {}})));
}


TEST(Hoi4worldStatesHoi4statesconverter, VictoryPointsArePlacedInHighestValueSignificantProvinces)
{
   vic3::WorldBuilder world = vic3::WorldBuilder::CreateNullWorld()
                                  .AddTestStates({{1, 2, 3}, {4, 5, 6}})
                                  .AddStateRegions({
                                      {"STATE_ONE",
                                          vic3::StateRegion(
                                              {
                                                  {"x000005", "city"},
                                                  {"x000004", "port"},
                                                  {"x000003", "farm"},
                                                  {"x000002", "mine"},
                                                  {"x000001", "wood"},
                                              },
                                              {})},
                                  });
   mappers::WorldMapperBuilder world_mapper =
       std::move(mappers::WorldMapperBuilder::CreateNullMapper().AddTestProvinces(6));
   world_mapper.CopyToVicWorld(world);
   hoi4::WorldFrameworkBuilder world_framework =
       WorldFrameworkBuilder::CreateNullWorldFramework().AddTestLandProvinces(6);
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = world_framework.CopyProvinceDefinitions(),
   });
   const auto hoi4_states = ConvertStates(world.Build(), world_mapper.Build(), world_framework.Build(), {}, map_data);

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1, {.provinces = {10, 20, 30}, .victory_points = {{30, 1}}}),
           State(2, {.provinces = {40, 50, 60}, .victory_points = {{50, 1}}})));
}


TEST(Hoi4worldStatesHoi4statesconverter, VictoryPointsAreValuedAtHalfTotalFactoriesRoundedUp)
{
   vic3::WorldBuilder world = vic3::WorldBuilder::CreateNullWorld()
                                  .AddTestStates({{1, 2, 3}, {4, 5, 6}})
                                  .AddStateRegions({
                                      {"STATE_ONE",
                                          vic3::StateRegion(
                                              {
                                                  {"x000005", "city"},
                                                  {"x000004", "port"},
                                                  {"x000003", "farm"},
                                                  {"x000002", "mine"},
                                                  {"x000001", "wood"},
                                              },
                                              {})},
                                  })
                                  .AddBuildings({vic3::Building("", 1, 875'000), vic3::Building("", 2, 700'000)});
   mappers::WorldMapperBuilder world_mapper = std::move(
       mappers::WorldMapperBuilder::CreateNullMapper().AddTestProvinces(6).AddCountries({{1, "ONE"}, {2, "TWO"}}));
   world_mapper.CopyToVicWorld(world);
   hoi4::WorldFrameworkBuilder world_framework =
       WorldFrameworkBuilder::CreateNullWorldFramework().AddTestLandProvinces(6);
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = world_framework.CopyProvinceDefinitions(),
   });
   const auto hoi4_states = ConvertStates(world.Build(), world_mapper.Build(), world_framework.Build(), {}, map_data);

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1,
                                {.owner = "ONE",
                                    .provinces = {10, 20, 30},
                                    .victory_points = {{30, 3}},
                                    .civilian_factories = 3,
                                    .military_factories = 2,
                                    .air_base_level = 1}),
           State(2,
               {.owner = "TWO",
                   .provinces = {40, 50, 60},
                   .victory_points = {{50, 2}},
                   .civilian_factories = 2,
                   .military_factories = 2,
                   .air_base_level = 1})));
}


TEST(Hoi4worldStatesHoi4statesconverter, DebugVictoryPointsAreConverted)
{
   vic3::WorldBuilder world = vic3::WorldBuilder::CreateNullWorld()
                                  .AddStates({
                                      {1, vic3::State({.provinces = {1, 2, 3}})},
                                      {2, vic3::State({.provinces = {4, 5, 6}})},
                                  })
                                  .AddStateRegions({
                                      {"STATE_ONE",
                                          vic3::StateRegion(
                                              {
                                                  {"x000005", "city"},
                                                  {"x000004", "port"},
                                                  {"x000003", "farm"},
                                                  {"x000002", "mine"},
                                                  {"x000001", "wood"},
                                              },
                                              {})},
                                  });
   mappers::WorldMapperBuilder world_mapper =
       std::move(mappers::WorldMapperBuilder::CreateNullMapper().AddTestProvinces(6));
   world_mapper.CopyToVicWorld(world);
   hoi4::WorldFrameworkBuilder world_framework =
       WorldFrameworkBuilder::CreateNullWorldFramework().AddTestLandProvinces(6);
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = world_framework.CopyProvinceDefinitions(),
   });
   const auto hoi4_states = ConvertStates(world.Build(),
       world_mapper.Build(),
       world_framework.Build(),
       {},
       map_data,
       configuration::debugConfig);

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1, {.provinces = {10, 20, 30}, .victory_points = {{10, 1}, {20, 2}, {30, 3}}}),
           State(2, {.provinces = {40, 50, 60}, .victory_points = {{40, 4}, {50, 5}}})));
}


TEST(Hoi4worldStatesHoi4statesconverter, OwnerGetsCoreOnIncorporatedStates)
{
   vic3::WorldBuilder world =
       vic3::WorldBuilder::CreateNullWorld()
           .AddStates({
               {1, vic3::State({.owner_number = 1, .incorporated = true, .provinces = {1, 2, 3}})},
               {2, vic3::State({.owner_number = 2, .incorporated = true, .provinces = {4, 5, 6}})},
           })
           .AddTestStateRegions({{1, 2, 3}, {4, 5, 6}});
   mappers::WorldMapperBuilder world_mapper = std::move(
       mappers::WorldMapperBuilder::CreateNullMapper().AddTestProvinces(6).AddCountries({{1, "ONE"}, {2, "TWO"}}));
   world_mapper.CopyToVicWorld(world);
   hoi4::WorldFrameworkBuilder world_framework =
       WorldFrameworkBuilder::CreateNullWorldFramework().AddTestLandProvinces(6);
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = world_framework.CopyProvinceDefinitions(),
   });
   const auto hoi4_states = ConvertStates(world.Build(), world_mapper.Build(), world_framework.Build(), {}, map_data);

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1, {.owner = "ONE", .provinces = {10, 20, 30}, .cores = {"ONE"}}),
           State(2, {.owner = "TWO", .provinces = {40, 50, 60}, .cores = {"TWO"}})));
}


TEST(Hoi4worldStatesHoi4statesconverter, InfrastructureIsTransferredFromVic3State)
{
   vic3::WorldBuilder world =
       vic3::WorldBuilder::CreateNullWorld()
           .AddStates({
               {1, vic3::State({.owner_number = 1, .infrastructure = 123.4F, .provinces = {1, 2, 3}})},
               {2, vic3::State({.owner_number = 2, .infrastructure = 567.8F, .provinces = {4, 5, 6}})},
           })
           .AddTestStateRegions({{1, 2, 3}, {4, 5, 6}});
   mappers::WorldMapperBuilder world_mapper = std::move(
       mappers::WorldMapperBuilder::CreateNullMapper().AddTestProvinces(6).AddCountries({{1, "TAG"}, {2, "TWO"}}));
   world_mapper.CopyToVicWorld(world);
   hoi4::WorldFrameworkBuilder world_framework =
       WorldFrameworkBuilder::CreateNullWorldFramework().AddTestLandProvinces(6);
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = world_framework.CopyProvinceDefinitions(),
   });
   const auto hoi4_states = ConvertStates(world.Build(), world_mapper.Build(), world_framework.Build(), {}, map_data);

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(
           State(1, {.owner = "TAG", .provinces = {10, 20, 30}, .air_base_level = 0, .vic3_infrastructure = 123.4F}),
           State(2,
               {.owner = "TWO",
                   .provinces = {40, 50, 60},
                   .air_base_level = 1,
                   .vic3_infrastructure = 567.8F,
                   .infrastructure = 3})));
   EXPECT_THAT(hoi4_states.province_to_state_id_map,
       testing::UnorderedElementsAre(testing::Pair(10, 1),
           testing::Pair(20, 1),
           testing::Pair(30, 1),
           testing::Pair(40, 2),
           testing::Pair(50, 2),
           testing::Pair(60, 2)));
   EXPECT_THAT(hoi4_states.vic3_state_ids_to_hoi4_state_ids,
       testing::UnorderedElementsAre(testing::Pair(1, 1), testing::Pair(2, 2)));
   EXPECT_THAT(hoi4_states.hoi4_state_names_to_vic3_state_names,
       testing::UnorderedElementsAre(testing::Pair("STATE_1", "REGION_001"), testing::Pair("STATE_2", "REGION_002")));
}

}  // namespace hoi4
