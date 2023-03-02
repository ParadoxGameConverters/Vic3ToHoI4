#include "src/hoi4_world/states/state_categories.h"

#include <set>



std::string hoi4::StateCategories::GetBestCategory(int num_building_slots) const
{
   std::string category = "rural";
   for (auto [category_slots, category_name]: categories_)
   {
      if (num_building_slots <= category_slots)
      {
         category = category_name;
         break;
      }
   }

   return category;
}