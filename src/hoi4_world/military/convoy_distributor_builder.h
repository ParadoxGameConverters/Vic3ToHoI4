#ifndef SRC_HOI4WORLD_MILITARY_CONVOYDISTRIBUTORBUILDER_H
#define SRC_HOI4WORLD_MILITARY_CONVOYDISTRIBUTORBUILDER_H


#include <string_view>

#include "src/hoi4_world/military/convoy_distributor.h"

namespace hoi4
{

ConvoyDistributor BuildConvoyDistributor(std::string_view templates_filename);

}  // namespace hoi4


#endif  // SRC_HOI4WORLD_MILITARY_CONVOYDISTRIBUTORBUILDER_H
