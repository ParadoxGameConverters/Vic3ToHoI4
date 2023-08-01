#include "src/vic3_world/countries/vic3_country.h"

#include "src/vic3_world/world/vic3_world.h"

namespace vic3
{
std::set<std::string> Country::GetAcquiredTechnologies(const vic3::World& world) const
{
   const auto allTechs = world.GetAcquiredTechnologies();
   const auto maybeVal = allTechs.find(this->GetNumber());
   if (maybeVal != allTechs.end())
   {
      return maybeVal->second;
   }
   else
   {
      return {};
   }
}

}  // namespace vic3