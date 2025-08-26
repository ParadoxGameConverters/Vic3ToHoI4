#include "src/hoi4_world/roles/triggers/any_other_country_trigger.h"

#include <algorithm>



namespace hoi4
{

bool AnyOtherCountryTrigger::IsValid(const Context& context, const World& world) const
{
   return std::ranges::any_of(world.GetCountries().begin(),
       world.GetCountries().end(),
       [context, &world, this](const auto& country) {
          // Check if all child triggers are valid for the other country
          return std::ranges::all_of(children_.begin(),
              children_.end(),
              [context, &world, &country](const std::unique_ptr<Trigger>& a) {
                 // Skip the country itself
                 const CountryScope* maybe_country = std::get_if<CountryScope>(&context.from);
                 if (maybe_country && maybe_country->country.GetTag() == country.second.GetTag())
                 {
                    return false;
                 }

                 const Context new_context{
                     .root = context.root,
                     .this_scope = CountryScope{.country = country.second},
                     .prev = context.this_scope,
                     .from = context.from,
                 };
                 return a->IsValid(new_context, world);
              });
       });
}


bool AnyOtherCountryTrigger::operator==(const AnyOtherCountryTrigger& rhs) const
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


bool AnyOtherCountryTrigger::operator<(const AnyOtherCountryTrigger& rhs) const
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


std::unique_ptr<Trigger> AnyOtherCountryTrigger::Copy() const
{
   std::vector<std::unique_ptr<Trigger>> children;
   for (const auto& child: children_)
   {
      children.push_back(child->Copy());
   }

   return std::make_unique<AnyOtherCountryTrigger>(std::move(children));
}


void PrintTo(const AnyOtherCountryTrigger& trigger, std::ostream* os)
{
   *os << "AnyOtherCountryTrigger: {\n";
   for (const std::unique_ptr<Trigger>& child: trigger.children_)
   {
      *os << child;
   }
   *os << "}\n";
}

}  // namespace hoi4