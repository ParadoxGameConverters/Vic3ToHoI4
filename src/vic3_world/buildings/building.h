#ifndef SRC_VIC3WORLD_BUILDINGS_BUILDING_H
#define SRC_VIC3WORLD_BUILDINGS_BUILDING_H



#include <string>



namespace vic3
{

class Building
{
  public:
   Building(std::string type, std::optional<int> state_number, float goods_sales_value):
       type_(std::move(type)),
       state_number_(state_number),
       goods_sales_value_(goods_sales_value)
   {
   }

   [[nodiscard]] const std::string& GetType() const { return type_; }
   [[nodiscard]] std::optional<int> GetStateNumber() const { return state_number_; }
   [[nodiscard]] float GetGoodsSalesVales() const { return goods_sales_value_; }

  private:
   std::string type_;
   std::optional<int> state_number_;
   float goods_sales_value_ = 0.0F;
};

}  // namespace vic3



#endif  // SRC_VIC3WORLD_BUILDINGS_BUILDING_H