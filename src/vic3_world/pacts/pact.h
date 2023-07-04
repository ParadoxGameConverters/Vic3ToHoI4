#ifndef SRC_VIC3WORLD_PACTS_PACT_H
#define SRC_VIC3WORLD_PACTS_PACT_H



#include <istream>



namespace vic3
{
class Pact
{
  public:
   Pact(const int first_id,
       const int second_id,
       const std::string action,
       const date start_date,
       const std::optional<int> forced_duration):
       first_id_(first_id),
       second_id_(second_id),
       action_(action),
       start_date_(start_date),
       forced_duration_(forced_duration)
   {
   }

   [[nodiscard]] int GetFirstId() const { return first_id_; }
   [[nodiscard]] int GetSecondId() const { return second_id_; }
   [[nodiscard]] std::string GetAction() const { return action_; }
   [[nodiscard]] date GetStartDate() const { return start_date_; }
   [[nodiscard]] std::optional<int> GetForcedDuration() const { return forced_duration_; }
   bool isSubjectRelationship() const
   {
      // This could be expanded into a mapper between Vic3 pact types and HoI4 autonomous_states
      const std::set<std::string> subjectPactTypes =
          {"protectorate", "puppet", "dominion", "personal_union", "vassal", "tributary"};
      return subjectPactTypes.find(action_) != subjectPactTypes.end();
   }

   std::partial_ordering operator<=>(const Pact&) const = default;

  private:
   // In vassal-type relationships, first_id_ is the id of the overlord, second_id_ is the id of the vassal.
   int first_id_ = 0;
   int second_id_ = 0;
   std::string action_;
   date start_date_;
   std::optional<int> forced_duration_;
};
}  // namespace vic3



#endif  // SRC_VIC3WORLD_PACTS_PACT_H