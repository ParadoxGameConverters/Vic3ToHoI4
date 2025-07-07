#ifndef SRC_HOI4WORLD_MAP_COASTALPROVINCES_H
#define SRC_HOI4WORLD_MAP_COASTALPROVINCES_H



#include <map>
#include <vector>



namespace hoi4
{

class CoastalProvinces
{
  public:
   CoastalProvinces() = default;
   explicit CoastalProvinces(std::map<int, std::vector<int>> coastal_provinces):
       coastal_provinces_(std::move(coastal_provinces))
   {
   }

   [[nodiscard]] const std::map<int, std::vector<int>>& GetCoastalProvinces() const { return coastal_provinces_; }
   [[nodiscard]] bool IsProvinceCoastal(int province_num) const { return coastal_provinces_.contains(province_num); }
   [[nodiscard]] bool contains(int province_num) const { return coastal_provinces_.contains(province_num); }

  private:
   std::map<int, std::vector<int>> coastal_provinces_;  // province, connecting sea provinces
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_MAP_COASTALPROVINCES_H