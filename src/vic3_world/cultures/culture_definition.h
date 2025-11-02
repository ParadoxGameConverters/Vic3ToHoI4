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


struct CultureDefinitionOptions
{
   std::string name;
   NameList name_list;
   std::string religion;
   std::set<std::string> traits;
   std::set<std::string> traditions;
   std::set<std::string> obsessions;
   std::string heritage;
   std::string language;
   std::set<std::string> ethnicities;
};


class CultureDefinition
{
  public:
   explicit CultureDefinition(std::string_view name,
       NameList name_list,
       std::string_view religion,
       std::set<std::string> traits,
       std::set<std::string> traditions,
       std::set<std::string> obsessions,
       std::string_view heritage,
       std::string_view language,
       std::set<std::string> ethnicities):
       name_(std::move(name)),
       name_list_(std::move(name_list)),
       religion_(std::move(religion)),
       traits_(std::move(traits)),
       traditions_(std::move(traditions)),
       obsessions_(std::move(obsessions)),
       heritage_(std::move(heritage)),
       language_(std::move(language)),
       ethnicities_(std::move(ethnicities))
   {
   }

   explicit CultureDefinition(CultureDefinitionOptions options):
       name_(std::move(options.name)),
       name_list_(std::move(options.name_list)),
       religion_(std::move(options.religion)),
       traits_(std::move(options.traits)),
       traditions_(std::move(options.traditions)),
       obsessions_(std::move(options.obsessions)),
       heritage_(std::move(options.heritage)),
       language_(std::move(options.language)),
       ethnicities_(std::move(options.ethnicities))
   {
   }

   [[nodiscard]] const std::string& GetName() const { return name_; }
   [[nodiscard]] const NameList& GetNameList() const { return name_list_; }
   [[nodiscard]] const std::string& GetReligion() const { return religion_; }
   [[nodiscard]] const std::set<std::string>& GetTraits() const { return traits_; }
   [[nodiscard]] const std::set<std::string>& GetTraditions() const { return traditions_; }
   [[nodiscard]] const std::set<std::string>& GetObsessions() const { return obsessions_; }
   [[nodiscard]] const std::string& GetHeritage() const { return heritage_; }
   [[nodiscard]] const std::string& GetLanguage() const { return language_; }
   [[nodiscard]] const std::set<std::string>& GetEthnicities() const { return ethnicities_; }

   std::partial_ordering operator<=>(const CultureDefinition&) const = default;

  private:
   std::string name_;
   NameList name_list_;
   std::string religion_;
   std::set<std::string> traits_;
   std::set<std::string> traditions_;
   std::set<std::string> obsessions_;
   std::string heritage_;
   std::string language_;
   std::set<std::string> ethnicities_;
};


}  // namespace vic3



#endif  // SRC_VIC3WORLD_CULTURES_CULTUREDEFINITION_H