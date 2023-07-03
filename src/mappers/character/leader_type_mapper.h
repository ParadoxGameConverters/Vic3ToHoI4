#ifndef SRC_MAPPERS_CHARACTER_LEADERTYPEMAPPER_H
#define SRC_MAPPERS_CHARACTER_LEADERTYPEMAPPER_H



#include <map>
#include <string>



namespace mappers
{
enum LeaderType
{
   kHeadOfState,
   kPrimeMinister,
   kCouncil
};

using PowerLaw = std::string;
using GovernanceLaw = std::string;
using PowerLeaderMap = std::map<PowerLaw, LeaderType>;
using GovernancePowerMap = std::map<GovernanceLaw, PowerLeaderMap>;


class LeaderTypeMapper
{
  public:
   explicit LeaderTypeMapper(GovernancePowerMap rules): rules_(std::move(rules)) {}

   [[nodiscard]] LeaderType GetCountryLeaderType(const std::set<std::string>& current_laws) const;

  private:
   GovernancePowerMap rules_;
};

}  // namespace mappers



#endif  // SRC_MAPPERS_CHARACTER_LEADERTYPEMAPPER_H