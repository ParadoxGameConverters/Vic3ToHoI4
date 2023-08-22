#pragma once
#include "src/vic3_world/world/vic3_world.h"

namespace mappers
{
struct CountryBalanceInformation
{
   int country_id;
   float raw_vic3_factories;
   // factories(n)/factories(n-1): can't gain an advantage over the next strongest player via conversion
   float max_ratio_by_competition;
   // log2 formula: above 120% of next strongest player, adjust max power down to give a more balanced game
   float max_ratio_by_overkill;

   // output "balanced" factory count
   float converted_vic3_factories;
   // ratio of "balanced" factory count to initial count, so we can multiply by this for each state
   float converted_factory_ratio;
   auto operator<=>(const CountryBalanceInformation& other) const = default;
};

/// maps factories, and potentially balances.
class IndustryMapper
{
  public:
   IndustryMapper(const vic3::World& source_world);
   float Map(const vic3::World& source_world, int state_number);

  private:
   std::vector<CountryBalanceInformation> countryBalances = {};
   float target_global_factories_ = 0.0F;
   float target_global_factory_ratio_ = 0.0F;

   void BalanceFactories();
};
}  // namespace mappers