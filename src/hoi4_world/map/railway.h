#ifndef SRC_HOI4WORLD_MAP_RAILWAY_H
#define SRC_HOI4WORLD_MAP_RAILWAY_H



#include <stdexcept>
#include <vector>



namespace hoi4
{

class Railway
{
  public:
#pragma optimize("",off)
   Railway(int level, const std::vector<int>& provinces): level_(level), provinces_(provinces)
   {
      if (provinces.size() < 2)
      {
         throw std::runtime_error("Railways must have at least two provinces.");
      }
   }
#pragma optimize("",on)

   [[nodiscard]] int GetLevel() const { return level_; }
   [[nodiscard]] const auto& GetProvinces() const { return provinces_; }
   [[nodiscard]] int GetLength() const { return static_cast<int>(provinces_.size()); }

  private:
   int level_ = 0;
   std::vector<int> provinces_;
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_MAP_RAILWAY_H