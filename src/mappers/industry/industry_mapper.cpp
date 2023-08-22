#include <numeric>
#include "industry_mapper.h"

namespace mappers
{

IndustryMapper::IndustryMapper(const vic3::World& source_world)
{
   // there's probably a faster way to do this

   for (const auto& state : source_world.GetStates())
   {
      if(state.second.GetOwnerNumber().has_value())
      {
         int owner = state.second.GetOwnerNumber().value();
         auto countryInfo =
             std::find_if(this->countryBalances.begin(), this->countryBalances.end(), [owner](const CountryBalanceInformation& balanceInfo) {
                return balanceInfo.country_id == owner;
             });
         if (countryInfo == this->countryBalances.end())
         {
            countryBalances.emplace_back(CountryBalanceInformation{owner,
                source_world.GetBuildings().GetTotalGoodSalesValueInState(state.first)});
         }
         else
         {
            countryInfo->raw_vic3_factories += source_world.GetBuildings().GetTotalGoodSalesValueInState(state.first);
         }
      }
   }

   std::sort(this->countryBalances.begin(), this->countryBalances.end(), [](const auto& a, const auto& b) {
      return a.raw_vic3_factories > b.raw_vic3_factories;
   });
   BalanceFactories();

   // TODO: make a config value
   target_global_factories_ = 1226.0F;

   float base_hoi4_factories = std::accumulate(this->countryBalances.begin(),
       this->countryBalances.end(),
       0.0F,
       [](float sum, CountryBalanceInformation country) {
          return sum + country.converted_vic3_factories;
       });

   target_global_factory_ratio_ = target_global_factories_ / base_hoi4_factories;
}

float IndustryMapper::Map(const vic3::World& source_world, int state_number)
{
   const int owner = source_world.GetStates().at(state_number).GetOwnerNumber().value();
   auto countryInfo = std::find_if(this->countryBalances.begin(),
              this->countryBalances.end(),
              [owner](const CountryBalanceInformation& balanceInfo) {
                 return balanceInfo.country_id == owner;
              });
   return source_world.GetBuildings().GetTotalGoodSalesValueInState(state_number) * target_global_factory_ratio_ *
          countryInfo->converted_factory_ratio;
}


float max_power_ratio_by_overkill(float country_power, float prev_country_power)
{
   float raw_power_ratio = country_power / prev_country_power;
   if (raw_power_ratio <= 1.2)
   {
      return raw_power_ratio;
   }
   else
   {
      // played with in desmos, this power curve looks good
      return 4 / 3 * log2(raw_power_ratio - 0.5) + 1;
   }
}

void IndustryMapper::BalanceFactories()
{
   {
       auto iter = this->countryBalances.rbegin();
      float prev_raw_vic3_factories = iter->raw_vic3_factories;
       for (; iter != this->countryBalances.rend(); ++iter)
      {
         iter->max_ratio_by_competition = iter->raw_vic3_factories / prev_raw_vic3_factories;
         iter->max_ratio_by_overkill = max_power_ratio_by_overkill(iter->raw_vic3_factories, prev_raw_vic3_factories);
         prev_raw_vic3_factories = iter->raw_vic3_factories;
      }
   }

   //and now, we adjust factory count
   {
      auto iter = this->countryBalances.rbegin();
      float prev_converted_vic3_factories = iter->raw_vic3_factories;
      for (; iter != this->countryBalances.rend(); ++iter)
      {
         iter->converted_vic3_factories =
             std::min(iter->max_ratio_by_competition, iter->max_ratio_by_overkill) * prev_converted_vic3_factories;
         prev_converted_vic3_factories = iter->converted_vic3_factories;
         iter->converted_factory_ratio = iter->converted_vic3_factories / iter->raw_vic3_factories;
      }
   }
}
}