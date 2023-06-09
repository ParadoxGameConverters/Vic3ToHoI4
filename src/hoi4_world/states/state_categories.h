#ifndef SRC_HOI4WORLD_STATES_STATECATEGORIES_H
#define SRC_HOI4WORLD_STATES_STATECATEGORIES_H



#include <map>
#include <string>



namespace hoi4
{

class StateCategories
{
  public:
   StateCategories() = default;
   explicit StateCategories(std::map<int, std::string> categories): categories_(std::move(categories)) {}

   [[nodiscard]] std::string GetBestCategory(int num_building_slots) const;
   [[nodiscard]] int GetNumSlots(const std::string& category_name_to_match) const;

  private:
   std::map<int, std::string> categories_;
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_STATES_STATECATEGORIES_H