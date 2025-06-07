#ifndef SRC_HOI4WORLD_ROLES_H
#define SRC_HOI4WORLD_ROLES_H



#include <string>
#include <vector>

#include "src/hoi4_world/decisions/decision.h"
#include "src/hoi4_world/decisions/decisions_category.h"
#include "src/hoi4_world/focus_trees/focus.h"
#include "src/hoi4_world/roles/repeat_focus.h"
#include "src/hoi4_world/roles/requirements/trigger_base.h"



namespace hoi4
{

struct RoleOptions
{
   std::string name;
   std::string category;
   std::unique_ptr<Trigger> requirement;
   float score;
   std::vector<std::string> blockers;
   std::vector<std::string> shared_focuses;
   std::vector<Focus> focuses;
   std::vector<RepeatFocus> repeat_focuses;
   std::vector<std::string> removed_focuses;
   std::vector<DecisionsCategory> decisions_categories;
   std::map<std::string, std::vector<Decision>> decisions_in_categories;
   std::vector<std::string> events;
};


class Role
{
  public:
   Role() = default;
   explicit Role(RoleOptions options):
       name_(std::move(options.name)),
       category_(std::move(options.category)),
       requirement_(std::move(options.requirement)),
       score_(options.score),
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

   [[nodiscard]] const std::string& GetName() const { return name_; }
   [[nodiscard]] const std::string& GetCategory() const { return category_; }
   [[nodiscard]] const Trigger& GetRequirement() const { return *requirement_; }
   [[nodiscard]] float GetScore() const { return score_; }
   [[nodiscard]] const std::vector<std::string>& GetBlockers() const { return blockers_; }
   [[nodiscard]] const std::vector<std::string>& GetSharedFocuses() const { return shared_focuses_; }
   [[nodiscard]] const std::vector<Focus>& GetFocuses() const { return focuses_; }
   [[nodiscard]] const std::vector<RepeatFocus>& GetRepeatFocuses() const { return repeat_focuses_; }
   [[nodiscard]] const std::vector<std::string>& GetRemovedFocuses() const { return removed_focuses_; }
   [[nodiscard]] const std::vector<DecisionsCategory>& GetDecisionsCategories() const { return decisions_categories_; }
   [[nodiscard]] const std::map<std::string, std::vector<Decision>>& GetDecisionsInCategories() const
   {
      return decisions_in_categories_;
   }
   [[nodiscard]] const std::vector<std::string>& GetEvents() const { return events_; }

   std::partial_ordering operator<=>(const Role&) const = default;

  private:
   std::string name_;
   std::string category_;
   std::unique_ptr<Trigger> requirement_;
   float score_;
   std::vector<std::string> blockers_;
   std::vector<std::string> shared_focuses_;
   std::vector<Focus> focuses_;
   std::vector<RepeatFocus> repeat_focuses_;
   std::vector<std::string> removed_focuses_;
   std::vector<DecisionsCategory> decisions_categories_;
   std::map<std::string, std::vector<Decision>> decisions_in_categories_;
   std::vector<std::string> events_;
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_ROLES_H