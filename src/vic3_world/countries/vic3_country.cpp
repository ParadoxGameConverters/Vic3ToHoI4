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

RankCategory Country::GetCountryRankCategory(const vic3::World& world) const
{
   const auto& rankings = world.GetCountryRankings();
   if (rankings.GetGreatPowers().contains(this->GetNumber()))
   {
      return RankCategory::kGreatPower;
   }
   else if (rankings.GetMajorPowers().contains(this->GetNumber()))
   {
      return RankCategory::kMajorPower;
   }
   else if (rankings.GetScoredCountries().contains(this->GetNumber()))
   {
      // todo: distinguish between minor and insignificant power
      return RankCategory::kMinorPower;
   }
   else
   {
      return RankCategory::kDecentralizedPower;
   }
}

std::vector<Institution> Country::GetInstitutions(const vic3::World& world) const
{
   return world.GetInstitutions(this->GetNumber());
}

}  // namespace vic3