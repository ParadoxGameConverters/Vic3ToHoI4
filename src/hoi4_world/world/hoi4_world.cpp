#include "src/hoi4_world/world/hoi4_world.h"

#include <ranges>



hoi4::World::World(const vic3::World& source_world, const mappers::CountryMapper& country_mapper)
{
   for (const auto& source_country: source_world.GetCountries() | std::views::values)
   {
      Country new_country(source_country, country_mapper);
      countries_.emplace(new_country.GetTag(), new_country);
   }
}