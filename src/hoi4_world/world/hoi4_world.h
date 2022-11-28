#ifndef SRC_HOI4WORLD_WORLD_HOI4WORLD_H
#define SRC_HOI4WORLD_WORLD_HOI4WORLD_H



#include <map>
#include <memory>
#include <string>

#include "external/commonItems/ModLoader/ModFilesystem.h"
#include "src/hoi4_world/countries/hoi4_country.h"
#include "src/hoi4_world/map/strategic_regions.h"
#include "src/hoi4_world/states/hoi4_state.h"
#include "src/mappers/country/country_mapper.h"
#include "src/mappers/provinces/province_mapper.h"
#include "src/vic3_world/world/vic3_world.h"



namespace hoi4
{

class World
{
  public:
   explicit World(commonItems::ModFilesystem hoi4_mod_filesystem,
       const vic3::World& source_world,
       const mappers::CountryMapper& country_mapper,
       const mappers::ProvinceMapper& province_mapper);
   explicit World(std::map<std::string, Country> countries,
       std::vector<State> states,
       StrategicRegions strategic_regions):
       countries_(std::move(countries)),
       states_(std::move(states)),
       strategic_regions_(std::make_unique<StrategicRegions>(strategic_regions))
   {
   }

   [[nodiscard]] const std::map<std::string, Country>& GetCountries() const { return countries_; }
   [[nodiscard]] const std::vector<State>& GetStates() const { return states_; }
   [[nodiscard]] const StrategicRegions& GetStrategicRegions() const { return *strategic_regions_; }

  private:
   std::map<std::string, Country> countries_;
   std::vector<State> states_;
   std::unique_ptr<StrategicRegions> strategic_regions_;
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_WORLD_HOI4WORLD_H