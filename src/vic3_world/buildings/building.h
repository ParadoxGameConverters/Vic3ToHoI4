#ifndef SRC_VIC3WORLD_BUILDINGS_BUILDING_H
#define SRC_VIC3WORLD_BUILDINGS_BUILDING_H



#include <optional>
#include <string>
#include <vector>


namespace vic3
{

// useful vanilla building types
constexpr const char* kBuildingTypeBarracks = "building_barracks";
constexpr const char* kBuildingTypeConscriptionCenter = "building_conscription_center";
constexpr const char* kBuildingTypeConstructionSector = "building_construction_sector";
constexpr const char* kBuildingTypeNavalBase = "building_naval_base";
constexpr const char* kBuildingTypeOilRig = "building_oil_rig";
constexpr const char* kBuildingTypePort = "building_port";
constexpr const char* kBuildingTypeShipyards = "building_shipyards";
constexpr const char* kBuildingTypeUrbanCenter = "building_urban_center";
constexpr const char* kBuildingTypeWarMachineIndustry = "building_war_machine_industry";


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
