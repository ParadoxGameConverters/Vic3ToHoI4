#ifndef SRC_VIC3WORLD_WORLD_VIC3WORLD_H
#define SRC_VIC3WORLD_WORLD_VIC3WORLD_H



#include <map>

#include "src/vic3_world/states/state.h"



namespace vic3
{

class World
{
  public:
   explicit World(std::map<int, State> states): states_(std::move(states)) {}


   [[nodiscard]] const auto& GetStates() const { return states_; }


  private:
   std::map<int, State> states_;
};

}  // namespace vic3



#endif  // SRC_VIC3WORLD_WORLD_VIC3WORLD_H