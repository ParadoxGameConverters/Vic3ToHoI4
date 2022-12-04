#ifndef SRC_MAPS_MAPDATAIMPORTER_H
#define SRC_MAPS_MAPDATAIMPORTER_H



#include "src/maps/map_data.h"



namespace maps
{

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
   void AddNeighbor(const std::string& main_province, const std::string& neighbor_province);
   void RemoveNeighbor(const std::string& main_province, const std::string& neighbor_province);
   void AddPointToBorder(const std::string& main_province, const std::string& neighbor_province, Point position);

   void ImportAdjacencies(const commonItems::ModFilesystem& mod_filesystem);

   std::map<std::string, std::set<std::string>> province_neighbors_;
   std::map<std::string, BordersWith> borders_;
   std::map<std::string, ProvincePoints> the_province_points_;

   ProvinceDefinitions province_definitions_;
   std::map<Point, std::string> points_to_provinces_;
};

}  // namespace maps



#endif  // SRC_MAPS_MAPDATAIMPORTER_H