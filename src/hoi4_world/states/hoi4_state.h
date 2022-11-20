#ifndef SRC_HOI4WORLD_STATES_HOI4STATE_H
#define SRC_HOI4WORLD_STATES_HOI4STATE_H



#include <set>



namespace hoi4
{

class State
{
  public:
   explicit State(int id, std::set<int> provinces): id_(id), provinces_(std::move(provinces)) {}

   [[nodiscard]] int GetId() const { return id_; }
   [[nodiscard]] const std::set<int>& GetProvinces() const { return provinces_; }

   std::strong_ordering operator<=>(const State&) const = default;

  private:
   int id_;
   std::set<int> provinces_;
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_STATES_HOI4STATE_H