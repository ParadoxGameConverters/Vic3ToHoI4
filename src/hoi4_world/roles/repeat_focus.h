#ifndef SRC_HOI4WORLD_ROLES_REPEATFOCUS_H
#define SRC_HOI4WORLD_ROLES_REPEATFOCUS_H



#include <memory>
#include <string>
#include <vector>

#include "src/hoi4_world/focus_trees/focus.h"
#include "src/hoi4_world/roles/requirements/trigger_base.h"
#include "src/hoi4_world/world/hoi4_world.h"



namespace hoi4
{

struct RepeatFocus
{
  public:
   RepeatFocus(std::unique_ptr<Trigger> requirement, std::vector<Focus> focuses):
       requirement_(std::move(requirement)),
       focuses_(std::move(focuses))
   {
   }

   [[nodiscard]] const Trigger& GetRequirement() const { return *requirement_; }
   [[nodiscard]] const std::vector<Focus>& GetFocuses() const { return focuses_; }

   bool operator==(const RepeatFocus& b) const = default;

   // This allows the Google test framework to print human-readable RepeatFocuses if a test fails.
   friend void PrintTo(const RepeatFocus& repeat_focus, std::ostream* os);

  private:
   std::unique_ptr<Trigger> requirement_;
   std::vector<Focus> focuses_;
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_ROLES_REPEATFOCUS_H