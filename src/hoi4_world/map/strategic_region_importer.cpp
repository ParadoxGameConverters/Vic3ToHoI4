#include "src/hoi4_world/map/strategic_region_importer.h"

#include <fstream>

#include "external/commonItems/CommonFunctions.h"
#include "external/commonItems/CommonRegexes.h"
#include "external/commonItems/ParserHelpers.h"
#include "external/fmt/include/fmt/format.h"



hoi4::StrategicRegionImporter::StrategicRegionImporter()
{
   state_region_parser_.registerKeyword("strategic_region", [this](std::istream& theStream) {
      map_region_parser_.parseStream(theStream);
   });
   state_region_parser_.registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);

   map_region_parser_.registerKeyword("id", [this](std::istream& input_stream) {
      id_ = commonItems::getInt(input_stream);
   });
   map_region_parser_.registerKeyword("name", [this](std::istream& input_stream) {
      name_ = commonItems::getString(input_stream);
   });
   map_region_parser_.registerKeyword("provinces", [this](std::istream& input_stream) {
      old_provinces_ = commonItems::getInts(input_stream);
   });
   map_region_parser_.registerKeyword("static_modifiers", [this](std::istream& input_stream) {
      const commonItems::assignments staticModifierStrings(input_stream);
      static_modifiers_ = staticModifierStrings.getAssignments();
   });
   map_region_parser_.registerKeyword("naval_terrain", [this](std::istream& input_stream) {
      naval_terrain_ = commonItems::getString(input_stream);
   });
   map_region_parser_.registerKeyword("weather", [this](std::istream& input_stream) {
      const commonItems::stringOfItem weather_string(input_stream);
      weather_ = weather_string.getString();
   });
   map_region_parser_.registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}


hoi4::StrategicRegion hoi4::StrategicRegionImporter::ImportStrategicRegion(std::string_view filename)
{
   id_ = 0;
   name_.clear();
   old_provinces_.clear();
   static_modifiers_.clear();
   naval_terrain_.reset();
   weather_.clear();

   state_region_parser_.parseFile(filename);

   return StrategicRegion(StrategicRegionOptions{.filename = trimPath(std::string(filename)),
       .id = id_,
       .name = name_,
       .old_provinces = old_provinces_,
       .static_modifiers = static_modifiers_,
       .naval_terrain = naval_terrain_,
       .weather = weather_});
}