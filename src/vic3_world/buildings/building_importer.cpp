#include "src/vic3_world/buildings/building_importer.h"

#include "external/commonItems/ParserHelpers.h"



vic3::BuildingImporter::BuildingImporter()
{
   building_parser_.registerKeyword("building", [this](std::istream& input_stream) {
      type_ = commonItems::getString(input_stream);
   });
   building_parser_.registerKeyword("state", [this](std::istream& input_stream) {
      state_number_ = commonItems::getInt(input_stream);
   });
   building_parser_.registerKeyword("goods_sales", [this](std::istream& input_stream) {
      goods_sales_value_ = static_cast<float>(commonItems::getDouble(input_stream));
   });
}


vic3::Building vic3::BuildingImporter::ImportBuilding(std::istream& input_stream)
{
   type_.erase();
   state_number_.reset();
   goods_sales_value_ = 0.0F;

   building_parser_.parseStream(input_stream);

   return {type_, state_number_, goods_sales_value_};
}