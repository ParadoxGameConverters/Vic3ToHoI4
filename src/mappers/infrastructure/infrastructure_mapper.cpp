#include "src/mappers/infrastructure/infrastructure_mapper.h"

#include <cmath>
#include <numeric>

#include "src/vic3_world/states/vic3_state.h"
#include "external/fmt/include/fmt/format.h"

// Find root of a function fn using secant approximation.
template <typename T, typename U>
U find_root(const std::function<U(T)>& fn, T x_0, const std::function<T(U)>& x_1_func, int iterations)
{
   U y_0 = fn(x_0);
   T x_1 = x_1_func(y_0);
   U y_1 = fn(x_1);

   for (; iterations > 0 && std::abs(y_1) > 0.005; --iterations)
   {
      float new_x = x_1 - (y_1 * (x_1 - x_0) / (y_1 - y_0));

      x_0 = x_1;
      y_0 = y_1;
      x_1 = new_x;
      y_1 = fn(new_x);
   }
   return x_1;
}

namespace mappers
{
InfrastructureMapper::InfrastructureMapper(const std::map<int, vic3::State>& states)
{
   target_hoi_infra_per_state_ = 1098.0F/899;  // TODO: put this in a file somewhere
   int total_vic_infra =
       std::accumulate(states.begin(), states.end(), 0.0f, [](float f, std::map<int, vic3::State>::value_type s) {
          return f + s.second.GetInfrastructure();
       });

   hoi_infra_per_vic_infra_ = target_hoi_infra_per_state_ / (total_vic_infra / static_cast<float>(states.size()));

   fudge_factor_ = findFudgeFactor(states);

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

int InfrastructureMapper::map(float vic3Infrastructure)
{
    // all states have minimum 1 infra
   int result = std::round(
       std::clamp(fudge_factor_ + vic3Infrastructure * hoi_infra_per_vic_infra_, 0.0F, 4.0F));
   converted_hoi_infra_ += result;
   converted_hoi_states_ += 1;
   return 1 + result;
}

float InfrastructureMapper::findFudgeFactor(const std::map<int, vic3::State>& states)
{
   const auto function = [=](float fudge_factor) {
      const float new_infra =
          std::accumulate(states.begin(), states.end(), 0.0f, [=](float f, std::map<int, vic3::State>::value_type s) {
             return f + std::round(std::clamp(
                            fudge_factor + (s.second.GetInfrastructure() * hoi_infra_per_vic_infra_),
                            0.0F,
                            4.0F));
          });
      Log(LogLevel::Debug) << fmt::format("fudge factor of {} gives {} infrastructure ({} per state)",
          fudge_factor,
          new_infra,
          (new_infra / states.size()));
      return ((new_infra / states.size()) - target_hoi_infra_per_state_) * target_hoi_infra_per_state_;
   };

   return find_root<float,float>(
       function,
       0.0,
       [](float f) { // good initial of fudge factor = -(difference%)
          return -f;
       },
       5);

}

}  // namespace mappers