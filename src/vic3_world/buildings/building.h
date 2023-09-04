#ifndef SRC_VIC3WORLD_BUILDINGS_BUILDING_H
#define SRC_VIC3WORLD_BUILDINGS_BUILDING_H



#include <string>
#include <vector>


namespace vic3
{

// useful vanilla building types
namespace BuildingType
{
const std::string Barracks = "building_barracks";
const std::string ConscriptionCenter = "building_conscription_center";
const std::string ConstructionSector = "building_construction_sector";
const std::string NavalBase = "building_naval_base";
const std::string OilRig = "building_oil_rig";
const std::string Port = "building_port";
const std::string Shipyards = "building_shipyards";
const std::string UrbanCenter = "building_urban_center";
const std::string WarMachineIndustry = "building_war_machine_industry";

}  // namespace BuildingType

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
