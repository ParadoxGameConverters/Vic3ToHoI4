#include "src/hoi4_world/world/hoi4_world.h"



namespace hoi4
{

std::optional<Country> World::GetCountry(const std::string& tag) const
{
   if (const auto& country_iterator = countries_.find(tag); country_iterator != countries_.end())
   {
      return country_iterator->second;
   }
   return std::nullopt;
}

}  // namespace hoi4