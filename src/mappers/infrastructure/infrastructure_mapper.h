#pragma once
#include <map>

#include "src/vic3_world/states/vic3_state.h"

// maps state infrastructure
namespace mappers
{

class InfrastructureMapper
{
  public:
   using storage_type = float;

   InfrastructureMapper(const std::map<int, vic3::State>& states);

   explicit InfrastructureMapper(storage_type conversion_ratio):
       hoi_infra_per_vic_infra_(conversion_ratio),
       fudge_factor_(0.0F)
   {
   }

   InfrastructureMapper(storage_type conversion_ratio, float fudge_factor):
       hoi_infra_per_vic_infra_(conversion_ratio),
       fudge_factor_(fudge_factor)
   {
   }

   int map(float vic3Infrastructure);

   storage_type getStorage() { return hoi_infra_per_vic_infra_; };

   float getTargetInfrastructure() { return target_hoi_infra_per_state_; }
   float getConvertedInfrastructure() { return static_cast<float>(converted_hoi_infra_) / converted_hoi_states_; }
   float GetFudgeFactor() { return fudge_factor_; }

  private:
   storage_type hoi_infra_per_vic_infra_ = 0.0f;
   float target_hoi_infra_per_state_ = 0.0F;
   int converted_hoi_infra_ = 0;
   int converted_hoi_states_ = 0;
   /// <summary>
   /// Usually, a naïve conversion doesn't result in a very close match to vanilla hoi's infrastructure count.
   /// We adjust the infrastructure in each province until we get close.
   /// </summary>
   float fudge_factor_ = 0.0F;

   float findFudgeFactor(const std::map<int, vic3::State>& states);
};
}  // namespace mappers