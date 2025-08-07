#ifndef SRC_MAPS_PROVINCEDEFINITIONS_H
#define SRC_MAPS_PROVINCEDEFINITIONS_H



#include <external/commonItems/Color.h>

#include <map>
#include <optional>
#include <set>



namespace maps
{

struct ProvinceDefinitionsOptions
{
   std::set<std::string> land_provinces;
   std::set<std::string> sea_provinces;
   std::map<std::string, std::string> terrain_types;
   std::map<std::string, std::string> continents;
   // colors are a packed integer to work around some issues. If you can get colors to work directly, please replace
   // this hack.
   std::map<int, std::string> color_to_province_map;
};


class ProvinceDefinitions
{
  public:
   ProvinceDefinitions() = default;
   ProvinceDefinitions(ProvinceDefinitionsOptions options):
       land_provinces_(std::move(options.land_provinces)),
       sea_provinces_(std::move(options.sea_provinces)),
       terrain_types_(options.terrain_types),
       continents_(std::move(options.continents)),
       color_to_province_map_(std::move(options.color_to_province_map))
   {
   }

   [[nodiscard]] const std::set<std::string>& GetLandProvinces() const { return land_provinces_; }
   [[nodiscard]] bool IsLandProvince(const std::string& province) const { return land_provinces_.contains(province); }
   [[nodiscard]] const std::set<std::string>& GetSeaProvinces() const { return sea_provinces_; }
   [[nodiscard]] bool IsSeaProvince(const std::string& province) const { return sea_provinces_.contains(province); }

   [[nodiscard]] std::optional<std::string> GetProvinceFromColor(const commonItems::Color& color) const;
   [[nodiscard]] std::optional<std::string> GetTerrainType(const std::string& province) const;
   [[nodiscard]] std::optional<std::string> GetContinent(const std::string& province) const;

  private:
   std::set<std::string> land_provinces_;
   std::set<std::string> sea_provinces_;
   std::map<std::string, std::string> terrain_types_;
   std::map<std::string, std::string> continents_;
   // colors are a packed integer to work around some issues. If you can get colors to work directly, please replace
   // this hack.
   std::map<int, std::string> color_to_province_map_;
};

}  // namespace maps



#endif  // SRC_MAPS_PROVINCEDEFINITIONS_H