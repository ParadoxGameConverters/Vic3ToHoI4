#ifndef SRC_MAPPERS_PROVINCEMAPPER_H
#define SRC_MAPPERS_PROVINCEMAPPER_H



#include <map>
#include <set>
#include <vector>

#include "src/mappers/provinces/province_mapping_types.h"



namespace mappers
{

class ProvinceMapper
{
  public:
   explicit ProvinceMapper(Vic3ToHoI4ProvinceMapping vic3_to_hoI4_province_map,
       HoI4ToVic3ProvinceMapping hoi4_to_vic3_province_map):
       vic3_to_hoI4_province_map_(std::move(vic3_to_hoI4_province_map)),
       hoi4_to_vic3_province_map_(std::move(hoi4_to_vic3_province_map))
   {
   }

   [[nodiscard]] std::vector<int> GetVic3ToHoI4ProvinceMapping(int vic3_province) const;
   [[nodiscard]] std::vector<int> GetHoI4ToVic3ProvinceMapping(int hoi4_province) const;
   [[nodiscard]] std::set<int> GetEquivalentVic3Provinces(const std::set<int>& hoi4_provinces) const;

   [[nodiscard]] bool IsVic3ProvinceMapped(int vic3_province_num) const
   {
      return vic3_to_hoI4_province_map_.contains(vic3_province_num);
   }

  private:
   Vic3ToHoI4ProvinceMapping vic3_to_hoI4_province_map_;
   HoI4ToVic3ProvinceMapping hoi4_to_vic3_province_map_;
};

}  // namespace mappers



#endif  // SRC_MAPPERS_PROVINCEMAPPER_H