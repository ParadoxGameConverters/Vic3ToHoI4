#include "out_world.h"

#include "src/out_hoi4/countries/out_countries.h"



void out::OutputWorld(std::string_view output_name, const hoi4::World& world)
{
   OutputCountries(output_name, world.GetCountries());
}