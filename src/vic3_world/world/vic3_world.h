#ifndef SRC_VIC3WORLD_WORLD_VIC3WORLD_H
#define SRC_VIC3WORLD_WORLD_VIC3WORLD_H



#include <map>

#include "src/vic3_world/countries/vic3_country.h"
#include "src/vic3_world/provinces/vic3_province_definitions.h"
#include "src/vic3_world/states/vic3_state.h"



namespace vic3
{

class World
{
  public:
   explicit World(std::map<int, Country> countries,
       std::map<int, State> states,
       ProvinceDefinitions province_definitions):
       countries_(std::move(countries)),
       states_(std::move(states)),
       province_definitions_(province_definitions)
   {
   }

   [[nodiscard]] const std::map<int, Country>& GetCountries() const { return countries_; }
   [[nodiscard]] const std::map<int, State>& GetStates() const { return states_; }
   [[nodiscard]] const ProvinceDefinitions GetProvinceDefinitions() const { return province_definitions_; }


  private:
   std::map<int, Country> countries_;
   std::map<int, State> states_;
   ProvinceDefinitions province_definitions_;
};

}  // namespace vic3



#endif  // SRC_VIC3WORLD_WORLD_VIC3WORLD_H