#ifndef SRC_HOI4WORLD_MILITARY_DIVISIONTEMPLATE_H
#define SRC_HOI4WORLD_MILITARY_DIVISIONTEMPLATE_H


#include <map>
#include <string>


namespace hoi4
{

using UnitCount = std::map<std::string, int>;

class DivisionTemplate
{
  public:
   DivisionTemplate(const std::string& n, UnitCount& bs, UnitCount& sp):
       name_(n),
       battalions_(std::move(bs)),
       support_(std::move(sp))
   {
   }

   [[nodiscard]] const std::string& GetName() const { return name_; }
   [[nodiscard]] const UnitCount& GetBattalions() const { return battalions_; }
   [[nodiscard]] const UnitCount& GetSupport() const { return support_; }

   std::strong_ordering operator<=>(const DivisionTemplate&) const = default;

  private:
   std::string name_;
   UnitCount battalions_;
   UnitCount support_;
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_MILITARY_DIVISIONTEMPLATE_H
