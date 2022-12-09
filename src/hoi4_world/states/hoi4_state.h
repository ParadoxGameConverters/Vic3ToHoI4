#ifndef SRC_HOI4WORLD_STATES_HOI4STATE_H
#define SRC_HOI4WORLD_STATES_HOI4STATE_H



#include <compare>
#include <optional>
#include <set>
#include <string>



namespace hoi4
{

class State
{
  public:
   explicit State(int id, std::optional<std::string> owner, std::set<int> provinces):
       id_(id),
       owner_(owner),
       provinces_(std::move(provinces))
   {
   }

   [[nodiscard]] int GetId() const { return id_; }
   [[nodiscard]] const std::optional<std::string>& GetOwner() const { return owner_; }
   [[nodiscard]] const std::set<int>& GetProvinces() const { return provinces_; }

   std::strong_ordering operator<=>(const State&) const = default;

  private:
   int id_;
   std::optional<std::string> owner_;
   std::set<int> provinces_;
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_STATES_HOI4STATE_H