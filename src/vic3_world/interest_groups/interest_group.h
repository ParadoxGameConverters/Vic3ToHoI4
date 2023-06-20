#ifndef SRC_VIC3WORLD_INTEREST_GROUPS_INTEREST_GROUP_H
#define SRC_VIC3WORLD_INTEREST_GROUPS_INTEREST_GROUP_H



#include <istream>



namespace vic3
{
class InterestGroup
{
  public:
   InterestGroup(std::string type, const int leader, const float clout, const bool in_government):
       type_(std::move(type)),
       leader_(leader),
       clout_(clout),
       in_government_(in_government)
   {
   }

   [[nodiscard]] const std::string& GetType() const { return type_; }

   [[nodiscard]] int GetCountryId() const { return country_id_; }
   [[nodiscard]] int GetLeader() const { return leader_; }
   [[nodiscard]] float GetClout() const { return clout_; }
   [[nodiscard]] bool IsInGovernment() const { return in_government_; }

   std::partial_ordering operator<=>(const InterestGroup&) const = default;

  private:
   std::string type_;

   int country_id_ = 0;
   int leader_ = 0;
   float clout_ = 0.0F;
   bool in_government_ = false;
};
}  // namespace vic3



#endif  // SRC_VIC3WORLD_INTEREST_GROUPS_INTEREST_GROUP_H