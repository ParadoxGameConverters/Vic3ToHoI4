#include "src/hoi4_world/roles/triggers/any_owned_state_trigger.h"

#include <algorithm>
#include <ranges>



namespace hoi4
{

bool AnyOwnedStateTrigger::IsValid(const Context& context, const World& world) const
{
   const CountryScope* maybe_country = std::get_if<CountryScope>(&context.this_scope);
   if (!maybe_country)
   {
      return false;
   }
   const Country& country = maybe_country->country;

   const auto state_is_valid = [context, country, &world, this](const State& state) {
      return std::ranges::all_of(children_.begin(),
          children_.end(),
          [context, country, &world, &state](const std::unique_ptr<Trigger>& a) {
             const Context new_context{
                 .root = context.root,
                 .this_scope = StateScope{.state = state},
                 .prev = context.this_scope,
                 .from = context.from,
             };
             return a->IsValid(new_context, world);
          });
   };

   std::vector<std::reference_wrapper<const State>> owned_states;
   for (const int owned_state: country.GetOwnedStates())
   {
      const State& state = world.GetStates().states.at(owned_state - 1);
      owned_states.emplace_back(state);
   }

   return std::ranges::any_of(owned_states, state_is_valid);
}


bool AnyOwnedStateTrigger::operator==(const AnyOwnedStateTrigger& rhs) const
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


bool AnyOwnedStateTrigger::operator<(const AnyOwnedStateTrigger& rhs) const
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


std::unique_ptr<Trigger> AnyOwnedStateTrigger::Copy() const
{
   std::vector<std::unique_ptr<Trigger>> children;
   for (const auto& child: children_)
   {
      children.push_back(child->Copy());
   }

   return std::make_unique<AnyOwnedStateTrigger>(std::move(children));
}


void PrintTo(const AnyOwnedStateTrigger& trigger, std::ostream* os)
{
   *os << "AnyOwnedStateTrigger: {\n";
   for (const std::unique_ptr<Trigger>& child: trigger.children_)
   {
      *os << child;
   }
   *os << "}\n";
}

}  // namespace hoi4