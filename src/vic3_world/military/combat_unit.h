#pragma once



#include <optional>
#include <string>



namespace vic3
{

struct CombatUnit
{
   int current_manpower = 0;
   std::optional<std::string> type;
   std::optional<int> country;
   std::optional<int> formation;

   std::strong_ordering operator<=>(const CombatUnit&) const = default;
};

}  // namespace vic3