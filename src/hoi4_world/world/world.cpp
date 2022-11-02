#include "src/hoi4_world/world/world.h"

#include <ranges>



hoi4::World::World(const vic3::World& src_world, const mappers::CountryMapper& country_mapper)
{
   for (const auto& src_country: src_world.GetCountries() | std::views::values)
   {
      Country new_country(src_country, country_mapper);
      countries_.emplace(new_country.GetTag(), new_country);
   }
}