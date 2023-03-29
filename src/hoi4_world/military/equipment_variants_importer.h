#ifndef SRC_HOI4WORLD_MILITARY_EQUIPMENTVARIANTSIMPORTER_H
#define SRC_HOI4WORLD_MILITARY_EQUIPMENTVARIANTSIMPORTER_H



#include <string_view>
#include <vector>

#include "src/hoi4_world/military/equipment_variant.h"



namespace hoi4
{

std::vector<EquipmentVariant> ImportEquipmentVariants(std::string_view variants_filename);

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_MILITARY_EQUIPMENTVARIANTSIMPORTER_H