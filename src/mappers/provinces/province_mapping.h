#ifndef SRC_MAPPERS_PROVINCE_PROVINCEMAPPING_H
#define SRC_MAPPERS_PROVINCE_PROVINCEMAPPING_H



#include <optional>
#include <string>
#include <vector>



namespace mappers
{

struct ProvinceMapping
{
   std::vector<std::string> vic3_provinces;
   std::vector<int> hoi4_provinces;
   std::optional<std::string> comment;
};

}  // namespace mappers



#endif  // SRC_MAPPERS_PROVINCE_PROVINCEMAPPING_H