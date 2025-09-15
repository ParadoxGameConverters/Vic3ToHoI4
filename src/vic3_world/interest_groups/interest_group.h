#ifndef SRC_VIC3WORLD_INTEREST_GROUPS_INTEREST_GROUP_H
#define SRC_VIC3WORLD_INTEREST_GROUPS_INTEREST_GROUP_H



#include <istream>
#include <set>
#include <string>

#include "src/support/named_type.h"



using InterestGroupCountryId = NamedType<int, struct InterestGroupCountryIdParameter>;
using InterestGroupLeader = NamedType<int, struct InterestGroupLeaderParameter>;
using InterestGroupClout = NamedType<float, struct InterestGroupCloutParameter>;
using InterestGroupInGovernment = NamedType<bool, struct InterestGroupInGovernmentParameter>;

namespace vic3
{
class InterestGroup
{
  public:
   InterestGroup(std::string type,
       const InterestGroupCountryId country_id,
       const InterestGroupLeader leader,
       const InterestGroupClout clout,
       const InterestGroupInGovernment in_government,
       std::set<std::string> ideologies):
       type_(std::move(type)),
       country_id_(country_id),
       leader_(leader),
       clout_(clout),
       in_government_(in_government),
       ideologies_(std::move(ideologies))
   {
   }

   [[nodiscard]] const std::string& GetType() const { return type_; }

   [[nodiscard]] int GetCountryId() const { return country_id_.Get(); }
   [[nodiscard]] int GetLeader() const { return leader_.Get(); }
   [[nodiscard]] float GetClout() const { return clout_.Get(); }
   [[nodiscard]] bool IsInGovernment() const { return in_government_.Get(); }
   [[nodiscard]] const std::set<std::string>& GetIdeologies() const { return ideologies_; }

   std::partial_ordering operator<=>(const InterestGroup&) const = default;
   bool operator==(const InterestGroup& other) const;

  private:
   std::string type_;

   InterestGroupCountryId country_id_{0};
   InterestGroupLeader leader_{0};
   InterestGroupClout clout_{0.0F};
   InterestGroupInGovernment in_government_{false};
   std::set<std::string> ideologies_;
};

}  // namespace vic3



#endif  // SRC_VIC3WORLD_INTEREST_GROUPS_INTEREST_GROUP_H