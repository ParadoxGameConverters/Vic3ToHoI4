#ifndef SRC_VIC3WORLD_BUILDINGS_BUILDING_H
#define SRC_VIC3WORLD_BUILDINGS_BUILDING_H



#include <optional>
#include <string>
#include <vector>

#include "src/support/named_type.h"



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



using GoodsSalesValue = NamedType<float, struct GoodsSalesValueParameter>;
using StaffingLevel = NamedType<float, struct StaffingLevelParameter>;


class Building
{
  public:
   Building(std::string_view type,
       std::optional<int> state_number,
       GoodsSalesValue goods_sales_value,
       StaffingLevel staffing_level,
       const std::vector<std::string>& production_methods):
       type_(type),
       state_number_(state_number),
       goods_sales_value_(goods_sales_value),
       staffing_level_(staffing_level),
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
   [[nodiscard]] float GetGoodsSalesValues() const { return goods_sales_value_.Get(); }
   [[nodiscard]] float GetStaffingLevel() const { return staffing_level_.Get(); }
   [[nodiscard]] const std::vector<std::string>& GetProductionMethods() const { return production_methods_; }

   auto operator<=>(const Building& other) const = default;
   bool operator==(const Building& other) const;

  private:
   std::string type_;
   std::optional<int> state_number_;
   GoodsSalesValue goods_sales_value_{0.0F};
   StaffingLevel staffing_level_{0.0F};
   std::vector<std::string> production_methods_;
};

}  // namespace vic3



#endif  // SRC_VIC3WORLD_BUILDINGS_BUILDING_H
