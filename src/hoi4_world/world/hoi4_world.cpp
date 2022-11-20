#include "src/hoi4_world/world/hoi4_world.h"

#include <ranges>

#include "external/commonItems/Log.h"
#include "src/hoi4_world/countries/hoi4_countries_converter.h"
#include "src/hoi4_world/states/hoi4_states_converter.h"



hoi4::World::World(const vic3::World& source_world,
    const mappers::CountryMapper& country_mapper,
    const mappers::ProvinceMapper& province_mapper)
{
   Log(LogLevel::Info) << "Creating Hoi4 world";
   Log(LogLevel::Progress) << "50%";

   Log(LogLevel::Info) << "\tConverting countries";
   CountriesConverter countries_converter;
   countries_ = countries_converter.ConvertCountries(source_world.GetCountries(), country_mapper);

   Log(LogLevel::Info) << "\tConverting states";
   Log(LogLevel::Progress) << "55%";
   StatesConverter states_converter;
   states_ = states_converter.ConvertStates(source_world.GetStates(),
       source_world.GetProvinceDefinitions(),
       province_mapper.GetHoi4ToVic3ProvinceMappings());
}