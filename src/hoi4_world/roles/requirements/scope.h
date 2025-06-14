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


struct StateScope
{
   const State& state;
};


using Scope = std::variant<CountryScope, StateScope>;

}  // namespace hoi4