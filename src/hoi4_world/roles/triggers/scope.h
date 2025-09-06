#pragma once



#include <variant>

#include "src/hoi4_world/countries/hoi4_country.h"
#include "src/hoi4_world/states/hoi4_state.h"



namespace hoi4
{

struct CountryScope
{
   const Country& country;
};


inline bool operator==(const CountryScope& lhs, const CountryScope& rhs)
{
   return lhs.country == rhs.country;
}


struct StateScope
{
   const State& state;
};


inline bool operator==(const StateScope& lhs, const StateScope& rhs)
{
   return lhs.state == rhs.state;
}


using Scope = std::variant<CountryScope, StateScope>;

}  // namespace hoi4