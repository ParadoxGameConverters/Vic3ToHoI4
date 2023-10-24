#ifndef SRC_HOI4WORLD_MILITARY_CONVOY_DISTRIBUTOR_H
#define SRC_HOI4WORLD_MILITARY_CONVOY_DISTRIBUTOR_H

#include <map>
#include <string>

#include "src/vic3_world/world/vic3_world.h"

namespace hoi4
{

class ConvoyDistributor
{
  public:
   ConvoyDistributor(int hoi, std::map<std::string, float>&& weights):
       hoi4_total_(hoi),
       vic3_total_(0),
       pm_weights_(weights)
   {
   }

   void CalculateStateWeights(const vic3::World& source_world);
   int ConvoysFromState(int id) const;

  private:
   int hoi4_total_;
   float vic3_total_;
   std::map<int, float> vic3_map_;
   std::map<std::string, float> pm_weights_;
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_MILITARY_CONVOY_DISTRIBUTOR_H
