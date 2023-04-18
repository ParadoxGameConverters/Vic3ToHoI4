#ifndef SRC_HOI4WORLD_MAP_RAILWAY_H
#define SRC_HOI4WORLD_MAP_RAILWAY_H



#include <stdexcept>
#include <vector>



namespace hoi4
{

class Railway
{
  public:
   Railway(int level, std::vector<int> provinces): level_(level), provinces_(std::move(provinces))
   {
      if (provinces_.size() < 2)
      {
         throw std::runtime_error("Railways must have at least two provinces.");
      }
   }

   [[nodiscard]] int GetLevel() const { return level_; }
   [[nodiscard]] const auto& GetProvinces() const { return provinces_; }
   [[nodiscard]] int GetLength() const { return static_cast<int>(provinces_.size()); }

   std::strong_ordering operator<=>(const Railway&) const = default;

  private:
   int level_ = 0;
   std::vector<int> provinces_;
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_MAP_RAILWAY_H