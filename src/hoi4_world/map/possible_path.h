#ifndef SRC_HOI4WORLD_MAP_POSSIBLEPATH_H
#define SRC_HOI4WORLD_MAP_POSSIBLEPATH_H



#include <optional>
#include <vector>



namespace hoi4
{

class PossiblePath
{
  public:
   PossiblePath() = default;
   explicit PossiblePath(int initial_province) { provinces_.push_back(initial_province); }

   void AddProvince(int province, double additional_cost)
   {
      provinces_.push_back(province);
      cost_ += additional_cost;
   }
   void ReplaceProvinces(std::vector<int> new_provinces) { provinces_ = std::move(new_provinces); }
   void SetLevel(int level) { level_ = level; }

   [[nodiscard]] int GetFirstProvince() const { return provinces_.front(); }
   [[nodiscard]] int GetLastProvince() const { return provinces_.back(); }
   [[nodiscard]] const std::vector<int>& GetProvinces() const { return provinces_; }
   [[nodiscard]] int GetLevel() const { return level_; }
   [[nodiscard]] double GetCost() const { return cost_; }

   // higher is prioritized where we want lower to be in priority queues, so reverse the meaning of <
   [[nodiscard]] bool operator<(const PossiblePath& rhs) const { return cost_ > rhs.cost_; }
   [[nodiscard]] bool operator==(const PossiblePath&) const = default;

  private:
   std::vector<int> provinces_;
   int level_ = 0;
   double cost_ = 0;
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_MAP_POSSIBLEPATH_H