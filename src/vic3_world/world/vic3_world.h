#ifndef SRC_VIC3WORLD_WORLD_VIC3WORLD_H
#define SRC_VIC3WORLD_WORLD_VIC3WORLD_H



#include <map>

#include "src/vic3_world/countries/vic3_country.h"
#include "src/vic3_world/provinces/vic3_province_definitions.h"
#include "src/vic3_world/states/vic3_state.h"



namespace vic3
{

struct WorldOptions
{
   std::map<int, Country> countries;
   std::map<int, State> states;
   ProvinceDefinitions province_definitions;
   std::map<int, std::vector<std::string>> acquired_technologies;
};


class World
{
  public:
   explicit World(WorldOptions world_options):
       countries_(std::move(world_options.countries)),
       states_(std::move(world_options.states)),
       province_definitions_(world_options.province_definitions),
       acquired_technologies_(std::move(world_options.acquired_technologies))
   {
   }

   [[nodiscard]] const std::map<int, Country>& GetCountries() const { return countries_; }
   [[nodiscard]] const std::map<int, State>& GetStates() const { return states_; }
   [[nodiscard]] const ProvinceDefinitions GetProvinceDefinitions() const { return province_definitions_; }


  private:
   std::map<int, Country> countries_;
   std::map<int, State> states_;
   ProvinceDefinitions province_definitions_;
   std::map<int, std::vector<std::string>> acquired_technologies_;
};

}  // namespace vic3



#endif  // SRC_VIC3WORLD_WORLD_VIC3WORLD_H