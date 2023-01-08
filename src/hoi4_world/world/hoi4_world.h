#ifndef SRC_HOI4WORLD_WORLD_HOI4WORLD_H
#define SRC_HOI4WORLD_WORLD_HOI4WORLD_H



#include <map>
#include <memory>
#include <string>

#include "src/hoi4_world/countries/hoi4_country.h"
#include "src/hoi4_world/map/strategic_regions.h"
#include "src/hoi4_world/states/hoi4_states.h"



namespace hoi4
{

class World
{
  public:
   explicit World(std::map<std::string, Country> countries, States states, StrategicRegions strategic_regions):
       countries_(std::move(countries)),
       states_(std::move(states)),
       strategic_regions_(std::move(strategic_regions))
   {
   }

   [[nodiscard]] const std::map<std::string, Country>& GetCountries() const { return countries_; }
   [[nodiscard]] const States& GetStates() const { return states_; }
   [[nodiscard]] const StrategicRegions& GetStrategicRegions() const { return strategic_regions_; }

  private:
   std::map<std::string, Country> countries_;
   States states_;
   StrategicRegions strategic_regions_;
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_WORLD_HOI4WORLD_H