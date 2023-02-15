#include "src/hoi4_world/technology/technologies.h"

#include <algorithm>
#include <numeric>



bool hoi4::Technologies::HasTechnology(const std::string& technology) const
{
   return std::ranges::any_of(technologies_by_limits_,
       [technology](const std::pair<std::optional<std::string>, std::set<std::string>>& technologies_by_limit) {
          return technologies_by_limit.second.contains(technology);
       });
}