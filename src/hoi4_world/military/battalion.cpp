#include "src/hoi4_world/military/battalion.h"


namespace hoi4
{

std::partial_ordering Battalion::operator<=>(const Battalion& other) const
{
   if (unit_type_ != other.unit_type_)
   {
      return unit_type_ <=> other.unit_type_;
   }
   if (equipment_scale_.Get() != other.equipment_scale_.Get())
   {
      return equipment_scale_.Get() <=> other.equipment_scale_.Get();
   }
   if (strength_.Get() != other.strength_.Get())
   {
      return strength_.Get() <=> other.strength_.Get();
   }
   if (location_ != other.location_)
   {
      if (!location_.has_value())
      {
         return std::partial_ordering::less;
      }
      if (!other.location_.has_value())
      {
         return std::partial_ordering::greater;
      }
      return location_.value() <=> other.location_.value();
   }
   return std::partial_ordering::equivalent;
}


bool Battalion::operator==(const Battalion& other) const
{
   if (unit_type_ != other.unit_type_)
   {
      return false;
   }
   if (equipment_scale_.Get() != other.equipment_scale_.Get())
   {
      return false;
   }
   if (strength_.Get() != other.strength_.Get())
   {
      return false;
   }
   if (location_ != other.location_)
   {
      return false;
   }
   return true;
}

}  // namespace hoi4