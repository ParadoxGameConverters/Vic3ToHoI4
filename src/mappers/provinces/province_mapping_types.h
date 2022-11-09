#ifndef SRC_MAPPERS_PROVINCEMAPPING_TYPES_H
#define SRC_MAPPERS_PROVINCEMAPPING_TYPES_H



#include <map>
#include <vector>



namespace mappers
{

using Vic3ToHoI4ProvinceMapping = std::map<int, std::vector<int>>;
using HoI4ToVic3ProvinceMapping = std::map<int, std::vector<int>>;

}  // namespace mappers



#endif  // SRC_MAPPERS_PROVINCEMAPPING_TYPES_H