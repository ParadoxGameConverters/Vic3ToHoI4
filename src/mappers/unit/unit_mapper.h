#ifndef SRC_MAPPERS_UNIT_UNITMAPPER_H
#define SRC_MAPPERS_UNIT_UNITMAPPER_H



#include <map>
#include <string>



namespace mappers
{

using UnitPointsMap = std::map<std::string, float>;
using PmToPointsMap = std::map<std::string, UnitPointsMap>;


class UnitMapper
{
  public:
   explicit UnitMapper(PmToPointsMap methods): methods_(std::move(methods)) {}

   [[nodiscard]] mappers::UnitPointsMap CalculateUnitPoints(const mappers::UnitPointsMap& pm_counts) const;

  private:
   PmToPointsMap methods_;
};

}  // namespace mappers



#endif  // SRC_MAPPERS_UNIT_UNITMAPPER_H
