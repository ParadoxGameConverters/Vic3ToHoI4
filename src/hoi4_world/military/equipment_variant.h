#ifndef SRC_HOI4WORLD_MILITARY_EQUIPMENTVARIANT_H
#define SRC_HOI4WORLD_MILITARY_EQUIPMENTVARIANT_H



#include <set>
#include <string>
#include <utility>
#include <vector>



namespace hoi4
{

class EquipmentVariant
{
  public:
   EquipmentVariant(std::string_view n,
       std::string_view t,
       std::set<std::string> required_techs,
       std::set<std::string> blocking_techs,
       std::vector<std::pair<std::string, std::string>> text_items):
       name_(n),
       type_(t),
       required_techs_(std::move(required_techs)),
       blocking_techs_(std::move(blocking_techs)),
       text_items_(std::move(text_items))
   {
   }
   [[nodiscard]] const std::string& GetName() const { return name_; }
   [[nodiscard]] const std::string& GetType() const { return type_; }
   [[nodiscard]] const std::set<std::string>& GetRequiredTechs() const { return required_techs_; }
   [[nodiscard]] const std::set<std::string>& GetBlockingTechs() const { return blocking_techs_; }
   [[nodiscard]] const std::vector<std::pair<std::string, std::string>>& GetTextItems() const { return text_items_; }


   std::strong_ordering operator<=>(const EquipmentVariant&) const = default;

  private:
   std::string name_;
   std::string type_;
   std::set<std::string> required_techs_;
   std::set<std::string> blocking_techs_;
   std::vector<std::pair<std::string, std::string>> text_items_;
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_MILITARY_EQUIPMENTVARIANT_H
