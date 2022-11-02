#ifndef SRC_HOI4WORLD_WORLD_WORLD_H
#define SRC_HOI4WORLD_WORLD_WORLD_H



#include <map>
#include <string>

#include "src/hoi4_world/countries/country.h"
#include "src/mappers/country/country_mapper.h"
#include "src/vic3_world/world/vic3_world.h"



namespace hoi4
{

class World
{
  public:
   explicit World(const vic3::World& source_world, const mappers::CountryMapper& country_mapper);
   [[nodiscard]] const std::map<std::string, Country>& GetCountries() const { return countries_; }

  private:
   std::map<std::string, Country> countries_;
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_WORLD_WORLD_H