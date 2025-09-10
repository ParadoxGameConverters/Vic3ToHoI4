#include "src/vic3_world/buildings/building_importer.h"

#include <external/commonItems/ParserHelpers.h>



vic3::BuildingImporter::BuildingImporter()
{
   building_parser_.registerKeyword("building", [this](std::istream& input_stream) {
      type_ = commonItems::getString(input_stream);
   });
   building_parser_.registerKeyword("state", [this](std::istream& input_stream) {
      int64_t temp_state_number = commonItems::getLlong(input_stream);
      if (temp_state_number != 4294967295)  // 4294967295 is -1, which indicates a dead building
      {
         state_number_ = static_cast<int>(temp_state_number);
      }
   });
   building_parser_.registerKeyword("goods_sales", [this](std::istream& input_stream) {
      goods_sales_value_ = static_cast<float>(commonItems::getDouble(input_stream));
   });
   building_parser_.registerKeyword("staffing", [this](std::istream& input_stream) {
      staffing_level_ = static_cast<float>(commonItems::getDouble(input_stream));
   });
   building_parser_.registerKeyword("production_methods", [this](std::istream& input_stream) {
      production_methods_ = commonItems::getStrings(input_stream);
   });
   building_parser_.IgnoreUnregisteredItems();
}


vic3::Building vic3::BuildingImporter::ImportBuilding(std::istream& input_stream)
{
   type_.erase();
   state_number_.reset();
   goods_sales_value_ = 0.0F;
   staffing_level_ = 0.0F;
   production_methods_.clear();

   building_parser_.parseStream(input_stream);

   return {type_,
       state_number_,
       GoodsSalesValue{goods_sales_value_},
       StaffingLevel{staffing_level_},
       production_methods_};
}
