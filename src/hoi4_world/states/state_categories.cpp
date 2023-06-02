#include "src/hoi4_world/states/state_categories.h"

#include <set>



std::string hoi4::StateCategories::GetBestCategory(int num_building_slots) const
{
   for (auto [category_slots, category_name]: categories_)
   {
      if (num_building_slots <= category_slots)
      {
         return category_name;
      }
   }

   return "rural";
}


int hoi4::StateCategories::GetNumSlots(const std::string& category_name_to_match) const
{
   for (auto [category_slots, category_name]: categories_)
   {
      if (category_name_to_match == category_name)
      {
         return category_slots;
      }
   }

   return 0;
}