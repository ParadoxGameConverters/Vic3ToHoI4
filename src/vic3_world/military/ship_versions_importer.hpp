#pragma once



#include <istream>
#include <map>
#include <string>



namespace vic3
{

std::map<int64_t, std::string> ImportShipVersions(std::istream& input);

}  // namespace vic3