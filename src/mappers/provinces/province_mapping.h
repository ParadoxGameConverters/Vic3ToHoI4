#ifndef SRC_MAPPERS_PROVINCE_PROVINCEMAPPING_H
#define SRC_MAPPERS_PROVINCE_PROVINCEMAPPING_H



#include <vector>



namespace mappers
{

struct ProvinceMapping
{
   std::vector<int> vic3_provinces;
   std::vector<int> hoi4_provinces;
};

}  // namespace mappers



#endif  // SRC_MAPPERS_PROVINCE_PROVINCEMAPPING_H