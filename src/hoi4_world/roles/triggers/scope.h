#pragma once



#include <variant>

#include "src/hoi4_world/countries/hoi4_country.h"
#include "src/hoi4_world/states/hoi4_state.h"
#include "src/vic3_world/cultures/culture_definition.h"



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


struct CultureScope
{
   const vic3::CultureDefinition& culture;
};


inline bool operator==(const CultureScope& lhs, const CultureScope& rhs)
{
   return lhs.culture == rhs.culture;
}


struct StateScope
{
   const State& state;
};


inline bool operator==(const StateScope& lhs, const StateScope& rhs)
{
   return lhs.state == rhs.state;
}


using Scope = std::variant<CountryScope, CultureScope, StateScope>;

}  // namespace hoi4