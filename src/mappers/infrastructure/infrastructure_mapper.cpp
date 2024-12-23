#include "src/mappers/infrastructure/infrastructure_mapper.h"

#include <external/commonItems/Log.h>
#include <external/fmt/include/fmt/format.h>

#include <algorithm>
#include <cmath>
#include <numeric>

#include "src/vic3_world/states/vic3_state.h"



namespace mappers
{
InfrastructureMapper::InfrastructureMapper(const std::map<int, vic3::State>& states)
{
   target_hoi_infra_per_state_ = 1098.0F / 899;
   int total_vic_infra =
       std::accumulate(states.begin(), states.end(), 0.0f, [](float f, std::map<int, vic3::State>::value_type s) {
          return f + s.second.GetInfrastructure();
       });

   hoi_infra_per_vic_infra_ = target_hoi_infra_per_state_ / (total_vic_infra / static_cast<float>(states.size()));

   fudge_factor_ = FindFudgeFactor(states);

   // tests can have zero sizes for states, so protect against NaNs
   if (!std::isfinite(fudge_factor_))
   {
      fudge_factor_ = 0.0F;
   }
   if (!std::isfinite(hoi_infra_per_vic_infra_))
   {
      hoi_infra_per_vic_infra_ = 0.0F;
   }

   converted_hoi_infra_ = 0;
}

int InfrastructureMapper::Map(float vic3Infrastructure)
{
   // we calculate the amount of additional infra (above 1), and then add 1 to the result at the end.
   // this works better* than just clamp(value, 1, 5) for some math reason that I haven't explored.
   // *better = less 5* infra provinces, more 2-4 infra provinces.
   int result = std::round(std::clamp(fudge_factor_ + vic3Infrastructure * hoi_infra_per_vic_infra_, 0.0F, 4.0F));
   converted_hoi_infra_ += result;
   converted_hoi_states_ += 1;
   // all states have minimum 1 infra.
   return 1 + result;
}

float InfrastructureMapper::FindFudgeFactor(const std::map<int, vic3::State>& states)
{
   const float target_hoi_infra_per_state = this->target_hoi_infra_per_state_;
   const float hoi_infra_per_vic_infra = this->hoi_infra_per_vic_infra_;
   const auto function = [states, target_hoi_infra_per_state, hoi_infra_per_vic_infra](float fudge_factor) {
      const float new_infra = std::accumulate(states.begin(),
          states.end(),
          0.0f,
          [hoi_infra_per_vic_infra, fudge_factor](float f, std::map<int, vic3::State>::value_type s) {
             return f + std::round(std::clamp(fudge_factor + (s.second.GetInfrastructure() * hoi_infra_per_vic_infra),
                            0.0F,
                            4.0F));
          });
      Log(LogLevel::Debug) << fmt::format("fudge factor of {} gives {} infrastructure ({} per state)",
          fudge_factor,
          new_infra,
          (new_infra / states.size()));
      return ((new_infra / states.size()) - target_hoi_infra_per_state) * target_hoi_infra_per_state;
   };

   return find_root<float, float>(
       function,
       0.0,
       [](float f) {  // good initial of fudge factor = -(difference%)
          return -f;
       },
       5);
}

}  // namespace mappers