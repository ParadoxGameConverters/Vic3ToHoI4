#ifndef HOI4WORLD_DIPLOMACY_HOI4WAR_H
#define HOI4WORLD_DIPLOMACY_HOI4WAR_H



#include <optional>
#include <set>
#include <string>



namespace hoi4
{

struct War
{
   std::string original_defender;
   std::set<std::string> extra_defenders;
   std::string original_attacker;
   std::set<std::string> extra_attackers;

   std::partial_ordering operator<=>(const War&) const = default;
};

}  // namespace hoi4



#endif  // HOI4WORLD_DIPLOMACY_HOI4WAR_H