#include "src/hoi4_world/states/hoi4_state.h"



void hoi4::State::SetHighestVictoryPointValue(int value)
{
   if (victory_points_.empty())
   {
      return;
   }

   auto highest_value_vp =
       std::ranges::max_element(victory_points_, [this](std::pair<int, int> a, std::pair<int, int> b) {
          return a.second < b.second;
       });
   highest_value_vp->second = value;
}