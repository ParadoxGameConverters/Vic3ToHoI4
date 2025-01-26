#ifndef SRC_OUTHOI4_MAP_OUTSUPPLYNODES_H
#define SRC_OUTHOI4_MAP_OUTSUPPLYNODES_H



#include <filesystem>
#include <set>
#include <string>



namespace out
{

void OutputSupplyNodes(const std::filesystem::path& output_name, const std::set<int>& supply_nodes);

}  // namespace out



#endif  // SRC_OUTHOI4_MAP_OUTSUPPLYNODES_H