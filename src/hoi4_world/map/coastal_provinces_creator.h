#ifndef SRC_HOI4WORLD_MAP_COASTALPROVINCESCREATOR_H
#define SRC_HOI4WORLD_MAP_COASTALPROVINCESCREATOR_H



#include <map>
#include <vector>

#include "src/hoi4_world/map/coastal_provinces.h"
#include "src/maps/map_data.h"
#include "src/support/named_type.h"



namespace hoi4
{

using LandProvinces = NamedType<std::set<std::string>, struct LandProvincesParameter>;
using SeaProvinces = NamedType<std::set<std::string>, struct SeaProvincesParameter>;



// create coastal province mapping from the map data and the names of land provinces and sea provinces.
std::map<int, std::vector<int>> CreateCoastalProvinces(const maps::MapData& map_data,
    const LandProvinces& land_provinces,
    const SeaProvinces& sea_provinces);

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_MAP_COASTALPROVINCESCREATOR_H