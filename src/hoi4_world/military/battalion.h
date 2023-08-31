#ifndef SRC_HOI4WORLD_MILITARY_BATTALION_H
#define SRC_HOI4WORLD_MILITARY_BATTALION_H

#include <string>

namespace hoi4
{

class Battalion
{
  public:
   Battalion(const std::string ut, int es, float str): unit_type_(ut), equipment_scale_(es), strength_(str) {}

   [[nodiscard]] const std::string& GetType() const { return unit_type_; }
   [[nodiscard]] int GetEquipmentScale() const { return equipment_scale_; }
   [[nodiscard]] const std::optional<int>& GetLocation() const { return location_; }
   [[nodiscard]] float GetStrength() const { return strength_; }
   void AddStrength(float s) { strength_ += s; }
   void SetLocation(int l) { location_ = l; }

   std::partial_ordering operator<=>(const Battalion&) const = default;

  private:
   std::string unit_type_;
   int equipment_scale_;
   float strength_;
   std::optional<int> location_;
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_MILITARY_BATTALION_H
