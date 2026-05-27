#include "src/hoi4_world/roles/triggers/any_state_trigger.h"

#include <algorithm>
#include <ranges>



namespace hoi4
{

bool AnyStateTrigger::IsValid(const Context& context, const World& world) const
{
   const auto state_is_valid = [context, &world, this](const State& state) {
      return std::ranges::all_of(children_.begin(),
          children_.end(),
          [context, &world, &state](const std::unique_ptr<Trigger>& a) {
             const Context new_context{
                 .root = context.root,
                 .this_scope = StateScope{.state = state},
                 .prev = context.this_scope,
                 .from = context.from,
             };
             return a->IsValid(new_context, world);
          });
   };

   return std::ranges::any_of(world.GetStates().states, state_is_valid);
}


std::vector<Scope> AnyStateTrigger::FindAllValid(const Context& context, const World& world) const
{
   std::vector<Scope> valid_scopes;

   const auto state_is_valid = [context, &world, this](const State& state) {
      return std::ranges::all_of(children_.begin(),
          children_.end(),
          [context, &world, &state](const std::unique_ptr<Trigger>& a) {
             const Context new_context{
                 .root = context.root,
                 .this_scope = StateScope{.state = state},
                 .prev = context.this_scope,
                 .from = context.from,
             };
             return a->IsValid(new_context, world);
          });
   };


   for (const State& state: world.GetStates().states)
   {
      if (std::ranges::all_of(children_.begin(),
              children_.end(),
              [context, &world, &state](const std::unique_ptr<Trigger>& a) {
                 const Context new_context{
                     .root = context.root,
                     .this_scope = StateScope{.state = state},
                     .prev = context.this_scope,
                     .from = context.from,
                 };
                 return a->IsValid(new_context, world);
              }))
      {
         valid_scopes.emplace_back(StateScope{.state = state});
      }
   }

   return valid_scopes;
}


bool AnyStateTrigger::operator==(const AnyStateTrigger& rhs) const
{
   if (children_.size() != rhs.children_.size())
   {
      return false;
   }

   for (unsigned i = 0; i < children_.size(); ++i)
   {
      if (*children_[i] != *rhs.children_[i])
      {
         return false;
      }
   }

   return true;
}


bool AnyStateTrigger::operator<(const AnyStateTrigger& rhs) const
{
   if (children_.size() < rhs.children_.size())
   {
      return true;
   }
   if (children_.size() > rhs.children_.size())
   {
      return false;
   }

   for (unsigned i = 0; i < children_.size(); ++i)
   {
      if (*children_[i] < *rhs.children_[i])
      {
         return true;
      }
      if (*children_[i] != *rhs.children_[i])
      {
         return false;
      }
   }

   return true;
}


std::unique_ptr<Trigger> AnyStateTrigger::Copy() const
{
   std::vector<std::unique_ptr<Trigger>> children;
   for (const auto& child: children_)
   {
      children.push_back(child->Copy());
   }

   return std::make_unique<AnyStateTrigger>(std::move(children));
}


void PrintTo(const AnyStateTrigger& trigger, std::ostream* os)
{
   *os << "AnyStateTrigger: {\n";
   for (const std::unique_ptr<Trigger>& child: trigger.children_)
   {
      *os << child;
   }
   *os << "}\n";
}

}  // namespace hoi4