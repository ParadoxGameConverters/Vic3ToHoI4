#ifndef SRC_HOI4WORLD_ROLES_H
#define SRC_HOI4WORLD_ROLES_H



#include <string>
#include <vector>

#include "src/hoi4_world/decisions/decision.h"
#include "src/hoi4_world/decisions/decisions_category.h"
#include "src/hoi4_world/events/event.h"
#include "src/hoi4_world/focus_trees/focus.h"
#include "src/hoi4_world/roles/alias_definition.h"
#include "src/hoi4_world/roles/repeat_focus.h"
#include "src/hoi4_world/roles/triggers/always_trigger.h"
#include "src/hoi4_world/roles/triggers/trigger_base.h"



namespace hoi4
{

struct RoleOptions
{
   std::string name;
   std::string category;
   std::unique_ptr<Trigger> trigger = std::make_unique<AlwaysTrigger>(false);
   float score;
   std::vector<std::string> blockers;
   std::vector<AliasDefinition> alias_definitions;
   std::vector<std::string> shared_focuses;
   std::vector<Focus> focuses;
   std::vector<RepeatFocus> repeat_focuses;
   std::vector<std::string> removed_focuses;
   std::vector<DecisionsCategory> decisions_categories;
   std::map<std::string, std::vector<Decision>> decisions_in_categories;
   std::vector<Event> events;
};


class Role
{
  public:
   Role() = default;
   explicit Role(RoleOptions options):
       name_(std::move(options.name)),
       category_(std::move(options.category)),
       trigger_(std::move(options.trigger)),
       score_(options.score),
       alias_definitions_(options.alias_definitions),
       blockers_(std::move(options.blockers)),
       shared_focuses_(std::move(options.shared_focuses)),
       focuses_(std::move(options.focuses)),
       repeat_focuses_(std::move(options.repeat_focuses)),
       removed_focuses_(std::move(options.removed_focuses)),
       decisions_categories_(std::move(options.decisions_categories)),
       decisions_in_categories_(std::move(options.decisions_in_categories)),
       events_(std::move(options.events))
   {
   }
   Role(const Role& rhs);
   Role& operator=(const Role& other);
   Role(Role&&) = default;
   Role& operator=(Role&&) = default;

   [[nodiscard]] const std::string& GetName() const { return name_; }
   [[nodiscard]] const std::string& GetCategory() const { return category_; }
   [[nodiscard]] const Trigger& GetTrigger() const { return *trigger_; }
   [[nodiscard]] float GetScore() const { return score_; }
   [[nodiscard]] const std::vector<std::string>& GetBlockers() const { return blockers_; }
   [[nodiscard]] const std::vector<AliasDefinition>& GetAliasDefinitions() const { return alias_definitions_; }
   [[nodiscard]] const std::vector<std::string>& GetSharedFocuses() const { return shared_focuses_; }
   [[nodiscard]] const std::vector<Focus>& GetFocuses() const { return focuses_; }
   [[nodiscard]] const std::vector<RepeatFocus>& GetRepeatFocuses() const { return repeat_focuses_; }
   [[nodiscard]] const std::vector<std::string>& GetRemovedFocuses() const { return removed_focuses_; }
   [[nodiscard]] const std::vector<DecisionsCategory>& GetDecisionsCategories() const { return decisions_categories_; }
   [[nodiscard]] const std::map<std::string, std::vector<Decision>>& GetDecisionsInCategories() const
   {
      return decisions_in_categories_;
   }
   [[nodiscard]] const std::vector<Event>& GetEvents() const { return events_; }

   std::partial_ordering operator<=>(const Role& other) const;
   bool operator==(const Role& other) const { return (*this <=> other) == std::partial_ordering::equivalent; }

   // This allows the Google test framework to print human-readable Role if a test fails.
   friend void PrintTo(const Role& role, std::ostream* os);

  private:
   std::string name_;
   std::string category_;
   std::unique_ptr<Trigger> trigger_ = std::make_unique<AlwaysTrigger>(false);
   float score_;
   std::vector<std::string> blockers_;
   std::vector<AliasDefinition> alias_definitions_;
   std::vector<std::string> shared_focuses_;
   std::vector<Focus> focuses_;
   std::vector<RepeatFocus> repeat_focuses_;
   std::vector<std::string> removed_focuses_;
   std::vector<DecisionsCategory> decisions_categories_;
   std::map<std::string, std::vector<Decision>> decisions_in_categories_;
   std::vector<Event> events_;
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_ROLES_H