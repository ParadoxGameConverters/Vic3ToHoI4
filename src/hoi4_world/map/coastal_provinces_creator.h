#ifndef SRC_HOI4WORLD_MAP_COASTALPROVINCESCREATOR_H
#define SRC_HOI4WORLD_MAP_COASTALPROVINCESCREATOR_H



#include <map>
#include <vector>

#include "src/hoi4_world/map/coastal_provinces.h"
#include "src/maps/map_data.h"



namespace hoi4
{

// create coastal province mapping from the map data and the names of land provinces and sea provinces.
CoastalProvinces::storage_type CreateCoastalProvinces(const maps::MapData& map_data,
    const std::set<std::string>& land_provinces,
    const std::set<std::string>& sea_provinces);

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_MAP_COASTALPROVINCESCREATOR_H