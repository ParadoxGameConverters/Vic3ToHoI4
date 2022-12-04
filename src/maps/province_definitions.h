#ifndef SRC_MAPS_PROVINCEDEFINITIONS_H
#define SRC_MAPS_PROVINCEDEFINITIONS_H



#include <map>
#include <optional>
#include <set>

#include "external/commonItems/Color.h"



namespace maps
{

class ProvinceDefinitions
{
  public:
   ProvinceDefinitions(std::set<std::string> land_provinces,
       std::set<std::string> sea_provinces,
       std::map<std::string, std::string> terrain_types,
       std::map<int, std::string> color_to_province_map):
       land_provinces_(std::move(land_provinces)),
       sea_provinces_(std::move(sea_provinces)),
       terrain_types_(terrain_types),
       color_to_province_map_(std::move(color_to_province_map))
   {
   }

   [[nodiscard]] const std::set<std::string>& GetLandProvinces() const { return land_provinces_; }
   [[nodiscard]] bool IsLandProvince(const std::string& province) const { return land_provinces_.contains(province); }
   [[nodiscard]] bool IsSeaProvince(const std::string& province) const { return sea_provinces_.contains(province); }

   [[nodiscard]] std::optional<std::string> GetProvinceFromColor(const commonItems::Color& color) const;
   [[nodiscard]] std::optional<std::string> GetTerrainType(const std::string& province) const;

  private:
   std::set<std::string> land_provinces_;
   std::set<std::string> sea_provinces_;
   std::map<std::string, std::string> terrain_types_;
   // colors are a packed integer to work around some issues. If you can get colors to work directly, please replace
   // this hack.
   std::map<int, std::string> color_to_province_map_;
};

}  // namespace maps



#endif  // SRC_MAPS_PROVINCEDEFINITIONS_H