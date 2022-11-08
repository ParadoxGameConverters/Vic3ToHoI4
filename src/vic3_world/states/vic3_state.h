#ifndef SRC_VIC3WORLD_STATES_VIC3STATE_H
#define SRC_VIC3WORLD_STATES_VIC3STATE_H



#include <set>



namespace vic3
{

class State
{
  public:
   explicit State(std::set<int> provinces): provinces_(std::move(provinces)) {}

   [[nodiscard]] const std::set<int>& GetProvinces() const { return provinces_; }

   bool operator==(const State&) const = default;

  private:
   std::set<int> provinces_;
};

}  // namespace vic3



#endif  // SRC_VIC3WORLD_STATES_VIC3STATE_H