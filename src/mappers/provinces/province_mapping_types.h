#ifndef SRC_MAPPERS_PROVINCEMAPPING_TYPES_H
#define SRC_MAPPERS_PROVINCEMAPPING_TYPES_H



#include <map>
#include <string>
#include <vector>



namespace mappers
{

using Vic3ToHoi4ProvinceMapping = std::map<std::string, std::vector<int>>;
using Hoi4ToVic3ProvinceMapping = std::map<int, std::vector<std::string>>;

}  // namespace mappers



#endif  // SRC_MAPPERS_PROVINCEMAPPING_TYPES_H