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
   explicit ProvinceMapper(Vic3ToHoi4ProvinceMapping vic3_to_hoi4_province_map,
       Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_map):
       vic3_to_hoi4_province_map_(std::move(vic3_to_hoi4_province_map)),
       hoi4_to_vic3_province_map_(std::move(hoi4_to_vic3_province_map))
   {
   }

   [[nodiscard]] std::vector<int> GetVic3ToHoi4ProvinceMapping(std::string_view vic3_province) const;
   [[nodiscard]] std::vector<std::string> GetHoi4ToVic3ProvinceMapping(int hoi4_province) const;
   [[nodiscard]] std::set<std::string> GetEquivalentVic3Provinces(const std::set<int>& hoi4_provinces) const;

   [[nodiscard]] Hoi4ToVic3ProvinceMapping GetHoi4ToVic3ProvinceMappings() const { return hoi4_to_vic3_province_map_; }

   void CheckAllVic3ProvincesMapped(const std::vector<std::string>& vic3_provinces) const;

  private:
   Vic3ToHoi4ProvinceMapping vic3_to_hoi4_province_map_;
   Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_map_;
};

}  // namespace mappers



#endif  // SRC_MAPPERS_PROVINCEMAPPER_H