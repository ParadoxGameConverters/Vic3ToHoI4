#ifndef SRC_VIC3WORLD_WORLD_VIC3WORLD_H
#define SRC_VIC3WORLD_WORLD_VIC3WORLD_H



#include <map>

#include "src/vic3_world/countries/vic3_country.h"
#include "src/vic3_world/states/vic3_state.h"



namespace vic3
{

class World
{
  public:
   explicit World(std::map<int, Country> countries, std::map<int, State> states):
       countries_(std::move(countries)),
       states_(std::move(states))
   {
   }

   [[nodiscard]] const auto& GetCountries() const { return countries_; }
   [[nodiscard]] const auto& GetStates() const { return states_; }


  private:
   std::map<int, Country> countries_;
   std::map<int, State> states_;
};

}  // namespace vic3



#endif  // SRC_VIC3WORLD_WORLD_VIC3WORLD_H