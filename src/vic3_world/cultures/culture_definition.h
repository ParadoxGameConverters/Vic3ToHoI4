#ifndef SRC_VIC3WORLD_CULTURES_CULTUREDEFINITION_H
#define SRC_VIC3WORLD_CULTURES_CULTUREDEFINITION_H

#include <set>
#include <string>



namespace vic3
{

struct NameList
{
   std::set<std::string> male_common_first;
   std::set<std::string> female_common_first;
   std::set<std::string> common_last;
   std::set<std::string> noble_last;
   std::set<std::string> male_regal_first;
   std::set<std::string> female_regal_first;

   std::partial_ordering operator<=>(const NameList&) const = default;
};

class CultureDefinition
{
  public:
   explicit CultureDefinition(std::string name,
       NameList name_list,
       std::set<std::string> traits,
       std::set<std::string> ethnicities):
       name_(std::move(name)),
       name_list_(std::move(name_list)),
       traits_(std::move(traits)),
       ethnicities_(std::move(ethnicities))
   {
   }

   [[nodiscard]] const std::string& GetName() const { return name_; }
   [[nodiscard]] const NameList& GetNameList() const { return name_list_; }
   [[nodiscard]] const std::set<std::string>& GetTraits() const { return traits_; }
   [[nodiscard]] const std::set<std::string>& GetEthnicities() const { return ethnicities_; }

   std::partial_ordering operator<=>(const CultureDefinition&) const = default;

  private:
   std::string name_;
   NameList name_list_;
   std::set<std::string> traits_;
   std::set<std::string> ethnicities_;
};


}  // namespace vic3



#endif  // SRC_VIC3WORLD_CULTURES_CULTUREDEFINITION_H