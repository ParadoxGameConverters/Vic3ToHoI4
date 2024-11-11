#pragma once



#include <functional>
#include <map>

#include "src/vic3_world/states/vic3_state.h"



// maps state infrastructure
namespace mappers
{

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


/// <summary>
/// Maps state infrastructure.
/// </summary>
/// <remarks>
/// Most internal data is represented in terms of "additional hoi infra above 1" because that makes a prettier result.
/// </remarks>
class InfrastructureMapper
{
  public:
   InfrastructureMapper(const std::map<int, vic3::State>& states);

   InfrastructureMapper(float conversion_ratio, float fudge_factor):
       hoi_infra_per_vic_infra_(conversion_ratio),
       fudge_factor_(fudge_factor)
   {
   }

   int Map(float vic3Infrastructure);

   float GetTargetInfrastructure() { return target_hoi_infra_per_state_; }
   float GetConvertedInfrastructure() { return static_cast<float>(converted_hoi_infra_) / converted_hoi_states_; }
   float GetConversionRatio() { return hoi_infra_per_vic_infra_; };
   float GetFudgeFactor() { return fudge_factor_; }

  private:
   /// hoi infra per vic infra. should be <1.
   /// We use this value instead of the more obvious vic_infra_per_hoi_infra
   /// so that when we set this to 0 during tests, we get a result of 0 hoi
   /// infra instead of infinite hoi infra during the conversion.
   float hoi_infra_per_vic_infra_ = 0.0f;
   /// <summary>
   /// target additional average hoi infra (amount above 1) per state.
   /// </summary>
   float target_hoi_infra_per_state_ = 0.0F;
   int converted_hoi_infra_ = 0;
   int converted_hoi_states_ = 0;
   /// <summary>
   /// Usually, a naïve conversion doesn't result in a very close match to vanilla hoi's infrastructure count.
   /// We adjust the infrastructure in each province until we get close.
   /// </summary>
   float fudge_factor_ = 0.0F;

   float FindFudgeFactor(const std::map<int, vic3::State>& states);
};
}  // namespace mappers