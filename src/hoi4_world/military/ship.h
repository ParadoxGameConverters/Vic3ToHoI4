#ifndef SRC_HOI4WORLD_MILITARY_SHIP_H
#define SRC_HOI4WORLD_MILITARY_SHIP_H

#include <string>

namespace hoi4
{

class Ship
{
  public:
   Ship(std::string_view name,
       std::string_view definition,
       std::string_view equipment,
       std::string_view legacy_equipment,
       std::string_view version):
       name_(name),
       definition_(definition),
       equipment_(equipment),
       legacy_equipment_(legacy_equipment),
       version_(version)
   {
   }

   [[nodiscard]] const std::string& GetName() const { return name_; }
   [[nodiscard]] const std::string& GetDefinition() const { return definition_; }
   [[nodiscard]] const std::string& GetEquipment() const { return equipment_; }
   [[nodiscard]] const std::string& GetLegacyEquipment() const { return legacy_equipment_; }
   [[nodiscard]] const std::string& GetVersion() const { return version_; }

   [[nodiscard]] std::partial_ordering operator<=>(const Ship&) const = default;

  private:
   std::string name_;
   std::string definition_;
   std::string equipment_;
   std::string legacy_equipment_;
   std::string version_;
};

}  // namespace hoi4


#endif  // SRC_HOI4WORLD_MILITARY_SHIP_H
