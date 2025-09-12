#include "src/vic3_world/buildings/building.h"



namespace vic3
{

bool Building::operator==(const Building& other) const
{
   if (type_ != other.type_)
   {
      return false;
   }
   if (state_number_ != other.state_number_)
   {
      return false;
   }
   if (goods_sales_value_.Get() != other.goods_sales_value_.Get())
   {
      return false;
   }
   if (staffing_level_.Get() != other.staffing_level_.Get())
   {
      return false;
   }
   if (production_methods_ != other.production_methods_)
   {
      return false;
   }
   return true;
}

}  // namespace vic3