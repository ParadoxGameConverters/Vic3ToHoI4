#ifndef SRC_MAPS_MAPDATAIMPORTER_H
#define SRC_MAPS_MAPDATAIMPORTER_H



#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#include "src/maps/map_data.h"
#include "src/support/named_type.h"



namespace maps
{

using MainProvince = NamedType<std::string, struct MainProvinceTag>;
using NeighborProvince = NamedType<std::string, struct NeighborProvinceTag>;



class MapDataImporter
{
  public:
   MapDataImporter(ProvinceDefinitions province_definitions): province_definitions_(std::move(province_definitions)) {}

   MapData ImportMapData(const commonItems::ModFilesystem& mod_filesystem);

  private:
   void ImportProvinces(const commonItems::ModFilesystem& mod_filesystem);
   void HandleNeighbor(const commonItems::Color& center_color,
       const commonItems::Color& other_color,
       const Point& position);
   void AddNeighbor(const MainProvince& main_province, const NeighborProvince& neighbor_province);
   void RemoveNeighbor(const MainProvince& main_province, const NeighborProvince& neighbor_province);
   void AddPointToBorder(const MainProvince& main_province, const NeighborProvince& neighbor_province, Point position);

   void ImportAdjacencies(const commonItems::ModFilesystem& mod_filesystem);

   std::map<std::string, std::set<std::string>> province_neighbors_;
   std::map<std::string, BordersWith> borders_;
   std::unordered_map<std::string, ProvincePoints> the_province_points_;

   ProvinceDefinitions province_definitions_;
   std::map<Point, std::string> points_to_provinces_;
};

}  // namespace maps



#endif  // SRC_MAPS_MAPDATAIMPORTER_H