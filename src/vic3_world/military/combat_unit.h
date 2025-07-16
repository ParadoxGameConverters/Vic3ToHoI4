#pragma once



#include <string>



namespace vic3
{

struct CombatUnit
{
   int current_manpower = 0;
   std::string type;
   int country;
   int formation;

   std::strong_ordering operator<=>(const CombatUnit&) const = default;
};

}  // namespace vic3