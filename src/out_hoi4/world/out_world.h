#ifndef SRC_OUTHOI4_WORLD_OUTWORLD_H
#define SRC_OUTHOI4_WORLD_OUTWORLD_H



#include <string_view>

#include "src/hoi4_world/world/hoi4_world.h"



namespace out
{

void OutputStats(const std::string& stat_name, double converted_value, double base_value);
void OutputWorld(std::string_view output_name, const hoi4::World& world);

}  // namespace out



#endif  // SRC_OUTHOI4_WORLD_OUTWORLD_H