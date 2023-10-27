#ifndef SRC_HOI4WORLD_MILITARY_SHIP_H
#define SRC_HOI4WORLD_MILITARY_SHIP_H

#include <string>

namespace hoi4
{

class Ship
{
  public:
   Ship(const std::string& name,
       const std::string& definition,
       const std::string& equipment,
       const std::string& version):
       name_(name),
       definition_(definition),
       equipment_(equipment),
       version_(version)
   {
   }

   const std::string& GetName() const { return name_; }
   const std::string& GetDefinition() const { return definition_; }
   const std::string& GetEquipment() const { return equipment_; }
   const std::string& GetVersion() const { return version_; }

   std::partial_ordering operator<=>(const Ship&) const = default;

  private:
   std::string name_;
   std::string definition_;
   std::string equipment_;
   std::string version_;
};

}  // namespace hoi4


#endif  // SRC_HOI4WORLD_MILITARY_SHIP_H
