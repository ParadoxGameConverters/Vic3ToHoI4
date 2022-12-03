#ifndef SRC_MAPS_MAPDATA_H
#define SRC_MAPS_MAPDATA_H



#include <map>
#include <optional>
#include <set>
#include <string_view>

#include "external/commonItems/Color.h"
#include "external/commonItems/ModLoader/ModFilesystem.h"
#include "src/maps/point.h"
#include "src/maps/province_definitions.h"
#include "src/maps/province_points.h"



namespace maps
{

using BorderPoints = std::vector<Point>;
using BordersWith = std::map<std::string, BorderPoints>;


class MapData
{
  public:
   MapData(const ProvinceDefinitions& province_definitions, const commonItems::ModFilesystem& mod_filesystem);

   [[nodiscard]] std::set<std::string> GetNeighbors(const std::string& province) const;
   [[nodiscard]] std::optional<Point> GetSpecifiedBorderCenter(const std::string& main_province,
       const std::string& neighbor) const;
   [[nodiscard]] std::optional<Point> GetAnyBorderCenter(const std::string& province) const;
   [[nodiscard]] std::optional<std::string> GetProvinceName(const Point& point) const;

   [[nodiscard]] std::optional<ProvincePoints> GetProvincePoints(const std::string& province) const;

  private:
   void ImportProvinces(const commonItems::ModFilesystem& mod_filesystem);
   void HandleNeighbor(const commonItems::Color& center_color,
       const commonItems::Color& other_color,
       const Point& position);
   void AddNeighbor(const std::string& main_province, const std::string& neighbor_province);
   void RemoveNeighbor(const std::string& main_province, const std::string& neighbor_province);
   void AddPointToBorder(const std::string& main_province, const std::string& neighbor_province, Point position);

   void ImportAdjacencies(const commonItems::ModFilesystem& mod_filesystem);
   void ImportAdjacencies(std::string_view path);

   std::map<std::string, std::set<std::string>> province_neighbors_;
   std::map<std::string, BordersWith> borders_;
   std::map<std::string, ProvincePoints> the_province_points_;

   ProvinceDefinitions province_definitions_;
   std::map<Point, std::string> points_to_provinces_;
};

}  // namespace maps



#endif  // SRC_MAPS_MAPDATA_H