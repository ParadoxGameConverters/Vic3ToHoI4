#ifndef SRC_VIC3WORLD_STATE_STATEREGION_H
#define SRC_VIC3WORLD_STATE_STATEREGION_H



#include <map>
#include <optional>
#include <string>



namespace vic3
{

using ProvinceId = std::string;
using ProvinceType = std::string;


class StateRegion
{
  public:
   explicit StateRegion(std::map<ProvinceId, ProvinceType> significant_provinces, std::set<ProvinceId> provinces):
       significant_provinces_(std::move(significant_provinces)),
       provinces_(std::move(provinces))
   {
   }

   [[nodiscard]] const std::map<ProvinceId, ProvinceType>& GetSignificantProvinces() const
   {
      return significant_provinces_;
   }
   [[nodiscard]] const std::set<ProvinceId>& GetProvinces() const { return provinces_; }

   std::strong_ordering operator<=>(const StateRegion&) const = default;

  private:
   std::map<ProvinceId, ProvinceType> significant_provinces_;  // province, type
   std::set<ProvinceId> provinces_;
};

}  // namespace vic3



#endif  // SRC_VIC3WORLD_STATE_STATEREGION_H