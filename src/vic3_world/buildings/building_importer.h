#ifndef SRC_VIC3WORLD_BUILDINGS_BUILDING_IMPORTER_H
#define SRC_VIC3WORLD_BUILDINGS_BUILDING_IMPORTER_H



#include <string>
#include <vector>

#include "external/commonItems/Parser.h"
#include "src/vic3_world/buildings/building.h"



namespace vic3
{

class BuildingImporter
{
  public:
   BuildingImporter();

   Building ImportBuilding(std::istream& input_stream);

  private:
   commonItems::parser building_parser_;

   std::string type_;
   std::optional<int> state_number_;
   float goods_sales_value_;
   float staffing_level_;
   std::vector<std::string> production_methods_;
};

}  // namespace vic3



#endif  // SRC_VIC3WORLD_BUILDINGS_BUILDING_IMPORTER_H
