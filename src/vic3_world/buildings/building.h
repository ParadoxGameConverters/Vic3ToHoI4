#ifndef SRC_VIC3WORLD_BUILDINGS_BUILDING_H
#define SRC_VIC3WORLD_BUILDINGS_BUILDING_H



#include <optional>
#include <string>
#include <vector>


namespace vic3
{

// useful vanilla building types
const std::string BuildingTypeBarracks = "building_barracks";
const std::string BuildingTypeConscriptionCenter = "building_conscription_center";
const std::string BuildingTypeConstructionSector = "building_construction_sector";
const std::string BuildingTypeNavalBase = "building_naval_base";
const std::string BuildingTypeOilRig = "building_oil_rig";
const std::string BuildingTypePort = "building_port";
const std::string BuildingTypeShipyards = "building_shipyards";
const std::string BuildingTypeUrbanCenter = "building_urban_center";
const std::string BuildingTypeWarMachineIndustry = "building_war_machine_industry";


class Building
{
  public:
   Building(std::string type,
       std::optional<int> state_number,
       float goods_sales_value,
       float staffing,
       const std::vector<std::string>& production_methods):
       type_(std::move(type)),
       state_number_(state_number),
       goods_sales_value_(goods_sales_value),
       staffing_level_(staffing),
       production_methods_(production_methods)
   {
   }
   Building(std::string type, std::optional<int> state_number, float goods_sales_value):
       type_(std::move(type)),
       state_number_(state_number),
       goods_sales_value_(goods_sales_value),
       production_methods_()
   {
   }

   [[nodiscard]] const std::string& GetType() const { return type_; }
   [[nodiscard]] std::optional<int> GetStateNumber() const { return state_number_; }
   [[nodiscard]] float GetGoodsSalesValues() const { return goods_sales_value_; }
   [[nodiscard]] float GetStaffingLevel() const { return staffing_level_; }
   [[nodiscard]] const std::vector<std::string>& GetProductionMethods() const { return production_methods_; }

   auto operator<=>(const Building& other) const = default;

  private:
   std::string type_;
   std::optional<int> state_number_;
   float goods_sales_value_ = 0.0F;
   float staffing_level_ = 0.0F;
   std::vector<std::string> production_methods_;
};

}  // namespace vic3



#endif  // SRC_VIC3WORLD_BUILDINGS_BUILDING_H
