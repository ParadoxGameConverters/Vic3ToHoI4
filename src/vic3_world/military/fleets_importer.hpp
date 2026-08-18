#pragma once



#include <istream>
#include <map>
#include <string>
#include <vector>



namespace vic3
{

std::map<int64_t, std::vector<int64_t>> ImportShips(std::istream& input_stream);
std::map<int64_t, std::vector<std::string>> AddShipTypes(const std::map<int64_t, std::string>& ship_versions,
    const std::map<int64_t, std::vector<int64_t>>& ships);

}  // namespace vic3