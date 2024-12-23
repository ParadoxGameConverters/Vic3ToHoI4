#ifndef SRC_HOI4WORLD_MAP_STRATEGICREGIONIMPORTER_H
#define SRC_HOI4WORLD_MAP_STRATEGICREGIONIMPORTER_H



#include <external/commonItems/Parser.h>

#include "src/hoi4_world/map/strategic_region.h"



namespace hoi4
{

class StrategicRegionImporter
{
  public:
   StrategicRegionImporter();

   [[nodiscard]] StrategicRegion ImportStrategicRegion(std::string_view filename);

  private:
   commonItems::parser state_region_parser_;
   commonItems::parser map_region_parser_;

   int id_ = 0;
   std::string name_;
   std::vector<int> old_provinces_;
   std::map<std::string, std::string> static_modifiers_;
   std::optional<std::string> naval_terrain_;
   std::string weather_;
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_MAP_STRATEGICREGIONIMPORTER_H
