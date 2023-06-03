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
   std::set<std::string> noble_last;
   std::set<std::string> common_last;
   std::optional<std::set<std::string>> male_regal_first;
   std::optional<std::set<std::string>> female_regal_first;
};

class CultureDefinition
{
  public:
   explicit CultureDefinition();

   [[nodiscard]] const NameList& GetNameList() const { return name_list_; }

  private:
   std::string name_;
   NameList name_list_;
   std::set<std::string> traits_;
   std::set<std::string> ethnicities_;
};


}  // namespace vic3



#endif  // SRC_VIC3WORLD_CULTURES_CULTUREDEFINITION_H