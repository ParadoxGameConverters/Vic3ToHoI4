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
      return RANK_GREAT_POWER;
   }
   else if (rankings.GetMajorPowers().contains(this->GetNumber()))
   {
      return RANK_MAJOR_POWER;
   }
   else if (rankings.GetScoredCountries().contains(this->GetNumber()))
   {
       // todo: distinguish between minor and insignificant power
      return RANK_MINOR_POWER;
   }
   else
   {
      return RANK_DECENTRALIZED_POWER;
   }
}

}  // namespace vic3