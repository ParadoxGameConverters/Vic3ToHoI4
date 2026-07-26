#pragma once



#include <istream>
#include <map>
#include <vector>



namespace vic3
{

std::map<int64_t, std::vector<int64_t>> ImportShips(std::istream& input_stream);

}  // namespace vic3