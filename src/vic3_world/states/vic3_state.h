#ifndef SRC_VIC3WORLD_STATES_VIC3STATE_H
#define SRC_VIC3WORLD_STATES_VIC3STATE_H



#include <set>



namespace vic3
{

struct StateOptions
{
   std::optional<int> owner_number;
   std::optional<std::string> owner_tag;
   std::set<int> provinces;
};


class State
{
  public:
   explicit State(StateOptions state_options):
       owner_number_(std::move(state_options.owner_number)),
       owner_tag_(std::move(state_options.owner_tag)),
       provinces_(std::move(state_options.provinces))
   {
   }

   [[nodiscard]] const std::optional<int>& GetOwnerNumber() const { return owner_number_; }
   [[nodiscard]] const std::optional<std::string>& GetOwnerTag() const { return owner_tag_; }
   [[nodiscard]] const std::set<int>& GetProvinces() const { return provinces_; }

   void SetOwnerTag(std::string_view owner_tag) { owner_tag_ = owner_tag; }

   bool operator==(const State&) const = default;

  private:
   std::optional<int> owner_number_;
   std::optional<std::string> owner_tag_;
   std::set<int> provinces_;
};

}  // namespace vic3



#endif  // SRC_VIC3WORLD_STATES_VIC3STATE_H