#ifndef SRC_VIC3WORLD_PROVINCES_VIC3PROVINCEDEFINITIONS_H
#define SRC_VIC3WORLD_PROVINCES_VIC3PROVINCEDEFINITIONS_H



#include <optional>
#include <string>
#include <vector>



namespace vic3
{

class ProvinceDefinitions
{
  public:
   explicit ProvinceDefinitions(std::vector<std::string> province_definitions):
       province_definitions_(std::move(province_definitions))
   {
   }

   [[nodiscard]] std::optional<std::string> GetProvinceDefinition(int province_number) const;

   [[nodiscard]] const std::vector<std::string>& GetProvinceDefinitions() const { return province_definitions_; }

  private:
   std::vector<std::string> province_definitions_;
};

}  // namespace vic3



#endif  // SRC_VIC3WORLD_PROVINCES_VIC3PROVINCEDEFINITIONS_H