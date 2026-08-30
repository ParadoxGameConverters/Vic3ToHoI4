#ifndef SRC_VIC3WORLD_PACTS_PACT_H
#define SRC_VIC3WORLD_PACTS_PACT_H



#include <external/commonItems/Date.h>

#include <istream>
#include <optional>
#include <set>



namespace vic3
{

// This could be expanded into a mapper between Vic3 pact types and HoI4 autonomous_states
static constexpr std::set<std::string>& GetSubjectPactTypes()
{
   static std::set<std::string> subject_pact_types = {
       "protectorate",
       "puppet",
       "dominion",
       "personal_union",
       "vassal",
       "tributary",
       "chartered_company",
       "colony",
   };

   return subject_pact_types;
}


struct PactPartners
{
   int first = 0;
   int second = 0;

   [[nodiscard]] std::partial_ordering operator<=>(const PactPartners&) const = default;
};


class Pact
{
  public:
   Pact(PactPartners partners, std::string action, date start_date, std::optional<int> forced_duration):
       partners_(partners),
       action_(std::move(action)),
       start_date_(start_date),
       forced_duration_(forced_duration)
   {
   }

   [[nodiscard]] PactPartners GetPartners() const { return partners_; }
   [[nodiscard]] std::string GetAction() const { return action_; }
   [[nodiscard]] date GetStartDate() const { return start_date_; }
   [[nodiscard]] std::optional<int> GetForcedDuration() const { return forced_duration_; }
   [[nodiscard]] bool IsSubjectRelationship() const { return GetSubjectPactTypes().contains(action_); }

   [[nodiscard]] std::partial_ordering operator<=>(const Pact&) const = default;

  private:
   // In vassal-type relationships, first_id_ is the id of the overlord, second_id_ is the id of the vassal.
   PactPartners partners_;
   std::string action_;
   date start_date_;
   std::optional<int> forced_duration_;
};
}  // namespace vic3



#endif  // SRC_VIC3WORLD_PACTS_PACT_H