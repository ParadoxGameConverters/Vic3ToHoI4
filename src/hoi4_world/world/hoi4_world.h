#ifndef SRC_HOI4WORLD_WORLD_HOI4WORLD_H
#define SRC_HOI4WORLD_WORLD_HOI4WORLD_H



#include <map>
#include <memory>
#include <string>

#include "external/commonItems/Localization/LocalizationDatabase.h"
#include "src/hoi4_world/countries/hoi4_country.h"
#include "src/hoi4_world/map/buildings.h"
#include "src/hoi4_world/map/strategic_regions.h"
#include "src/hoi4_world/states/hoi4_states.h"



namespace hoi4
{

struct WorldOptions
{
   std::map<std::string, Country> countries;
   States states;
   StrategicRegions strategic_regions;
   Buildings buildings;
};


class World
{
  public:
   explicit World(WorldOptions options):
       countries_(std::move(options.countries)),
       states_(std::move(options.states)),
       strategic_regions_(std::move(options.strategic_regions)),
       buildings_(std::move(options.buildings))
   {
   }

   [[nodiscard]] const std::map<std::string, Country>& GetCountries() const { return countries_; }
   [[nodiscard]] const States& GetStates() const { return states_; }
   [[nodiscard]] const StrategicRegions& GetStrategicRegions() const { return strategic_regions_; }
   [[nodiscard]] const Buildings& GetBuildings() const { return buildings_; }

  private:
   std::map<std::string, Country> countries_;
   States states_;
   StrategicRegions strategic_regions_;
   Buildings buildings_;
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_WORLD_HOI4WORLD_H