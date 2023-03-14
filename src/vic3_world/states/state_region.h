#ifndef SRC_VIC3WORLD_STATE_STATEREGION_H
#define SRC_VIC3WORLD_STATE_STATEREGION_H



#include <map>
#include <optional>
#include <string>



namespace vic3
{

class StateRegion
{
  public:
   explicit StateRegion(std::map<std::string, std::string> significant_provinces, std::set<std::string> provinces):
       significant_provinces_(std::move(significant_provinces)),
       provinces_(std::move(provinces))
   {
   }

   [[nodiscard]] const std::map<std::string, std::string>& GetSignificantProvinces() const
   {
      return significant_provinces_;
   }
   [[nodiscard]] const std::set<std::string>& GetProvinces() const { return provinces_; }

   std::strong_ordering operator<=>(const StateRegion&) const = default;

  private:
   std::map<std::string, std::string> significant_provinces_;  // province, type
   std::set<std::string> provinces_;
};

}  // namespace vic3



#endif  // SRC_VIC3WORLD_STATE_STATEREGION_H