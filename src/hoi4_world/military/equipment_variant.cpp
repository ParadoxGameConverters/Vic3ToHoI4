#include "src/hoi4_world/military/equipment_variant.h"



namespace hoi4
{

std::strong_ordering EquipmentVariant::operator<=>(const EquipmentVariant& other) const
{
   if (name_.Get() != other.name_.Get())
   {
      return name_.Get() <=> other.name_.Get();
   }
   if (type_.Get() != other.type_.Get())
   {
      return type_.Get() <=> other.type_.Get();
   }
   if (required_techs_ != other.required_techs_)
   {
      return required_techs_ <=> other.required_techs_;
   }
   if (blocking_techs_ != other.blocking_techs_)
   {
      return blocking_techs_ <=> other.blocking_techs_;
   }
   if (text_items_ != other.text_items_)
   {
      return text_items_ <=> other.text_items_;
   }

   return std::strong_ordering::equal;
}


bool EquipmentVariant::operator==(const EquipmentVariant& other) const
{
   if (name_.Get() != other.name_.Get())
   {
      return false;
   }
   if (type_.Get() != other.type_.Get())
   {
      return false;
   }
   if (required_techs_ != other.required_techs_)
   {
      return false;
   }
   if (blocking_techs_ != other.blocking_techs_)
   {
      return false;
   }
   if (text_items_ != other.text_items_)
   {
      return false;
   }

   return true;
}

}  // namespace hoi4