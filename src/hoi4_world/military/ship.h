#ifndef SRC_HOI4WORLD_MILITARY_SHIP_H
#define SRC_HOI4WORLD_MILITARY_SHIP_H

#include <string>

namespace hoi4
{

struct ShipOptions
{
   std::string name;
   std::string definition;
   std::string equipment;
   std::string legacy_equipment;
   std::string version;
};


class Ship
{
  public:
   explicit Ship(ShipOptions options):
       name_(options.name),
       definition_(options.definition),
       equipment_(options.equipment),
       legacy_equipment_(options.legacy_equipment),
       version_(options.version)
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
