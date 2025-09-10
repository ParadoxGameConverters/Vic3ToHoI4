#ifndef SRC_HOI4WORLD_MILITARY_BATTALION_H
#define SRC_HOI4WORLD_MILITARY_BATTALION_H



#include <optional>
#include <string>

#include "src/support/named_type.h"



namespace hoi4
{

using BattalionEquipmentScaleType = NamedType<int, struct BattalionEquipmentScaleParameter>;
using BattalionStrengthType = NamedType<float, struct BattalionStrengthParameter>;


class Battalion
{
  public:
   explicit Battalion(std::string_view unit_type,
       BattalionEquipmentScaleType equipment_scale,
       BattalionStrengthType strength):
       unit_type_(unit_type),
       equipment_scale_(equipment_scale),
       strength_(strength)
   {
   }

   [[nodiscard]] const std::string& GetType() const { return unit_type_; }
   [[nodiscard]] int GetEquipmentScale() const { return equipment_scale_.Get(); }
   [[nodiscard]] const std::optional<int>& GetLocation() const { return location_; }
   [[nodiscard]] float GetStrength() const { return strength_.Get(); }

   void AddStrength(float s) { strength_.Get() += s; }
   void SetLocation(int l) { location_ = l; }

   std::partial_ordering operator<=>(const Battalion&) const = default;

  private:
   std::string unit_type_;
   BattalionEquipmentScaleType equipment_scale_;
   BattalionStrengthType strength_;
   std::optional<int> location_;
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_MILITARY_BATTALION_H