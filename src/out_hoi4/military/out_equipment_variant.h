#ifndef SRC_OUTHOI4_MILITARY_OUTEQUIPMENTVARIANT_H
#define SRC_OUTHOI4_MILITARY_OUTEQUIPMENTVARIANT_H



#include <ostream>

#include "src/hoi4_world/military/equipment_variant.h"



namespace out
{

std::ostream& operator<<(std::ostream& out_stream, const hoi4::EquipmentVariant& equipment_variant);

}  // namespace out



#endif  // SRC_OUTHOI4_MILITARY_OUTEQUIPMENTVARIANT_H