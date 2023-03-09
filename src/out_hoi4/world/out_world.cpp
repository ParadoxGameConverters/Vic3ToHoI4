#include "out_world.h"

#include "src/out_hoi4/countries/out_countries.h"
#include "src/out_hoi4/localizations/out_localizations.h"
#include "src/out_hoi4/map/out_buildings.h"
#include "src/out_hoi4/map/out_rocket_sites.h"
#include "src/out_hoi4/map/out_strategic_regions.h"
#include "src/out_hoi4/states/out_states.h"



void out::OutputWorld(std::string_view output_name, const hoi4::World& world)
{
   OutputCountries(output_name, world.GetCountries());
   OutputStates(output_name, world.GetStates().states);
   OutputStrategicRegions(output_name, world.GetStrategicRegions());
   OutputBuildings(output_name, world.GetBuildings());
   OutputRocketSites(output_name, world.GetStates().states);
   OutputLocalizations(output_name, world.GetLocalizations());
}