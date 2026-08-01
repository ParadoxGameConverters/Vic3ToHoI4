#pragma once



#include <map>
#include <string>
#include <string_view>

#include "src/hoi4_world/military/ship.h"



namespace hoi4
{

std::map<std::string, hoi4::Ship> ImportShipMappings(std::string_view input_file);

}  // namespace hoi4