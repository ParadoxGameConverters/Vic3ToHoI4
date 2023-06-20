#ifndef SRC_VIC3WORLD_INTEREST_GROUPS_INTERESTGROUPSIMPORTER_H
#define SRC_VIC3WORLD_INTEREST_GROUPS_INTERESTGROUPSIMPORTER_H



#include <istream>

#include "src/vic3_world/interest_groups/interest_group.h"


namespace vic3
{

std::map<int, InterestGroup> ImportInterestGroups(std::istream& input_stream);

}  // namespace vic3



#endif  // SRC_VIC3WORLD_INTEREST_GROUPS_INTERESTGROUPSIMPORTER_H