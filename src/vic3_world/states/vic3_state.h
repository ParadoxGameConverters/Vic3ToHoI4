#ifndef SRC_VIC3WORLD_STATES_VIC3STATE_H
#define SRC_VIC3WORLD_STATES_VIC3STATE_H



#include <set>



namespace vic3
{

class State
{
  public:
   explicit State(std::optional<int> owner_number, std::set<int> provinces):
       owner_number_(std::move(owner_number)),
       provinces_(std::move(provinces))
   {
   }

   [[nodiscard]] const std::optional<int>& GetOwnerNumber() const { return owner_number_; }
   [[nodiscard]] const std::set<int>& GetProvinces() const { return provinces_; }

   bool operator==(const State&) const = default;

  private:
   std::optional<int> owner_number_;
   std::set<int> provinces_;
};

}  // namespace vic3



#endif  // SRC_VIC3WORLD_STATES_VIC3STATE_H