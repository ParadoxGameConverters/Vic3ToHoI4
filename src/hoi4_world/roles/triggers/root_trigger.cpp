#include "src/hoi4_world/roles/triggers/root_trigger.h"

#include <algorithm>
#include <ranges>



namespace hoi4
{

bool RootTrigger::IsValid(const Context& context, [[maybe_unused]] const World& world) const
{
   if (value_.starts_with("c:"))
   {
      const CountryScope* maybe_country = std::get_if<CountryScope>(&context.root);
      if (!maybe_country)
      {
         return false;
      }

      return maybe_country->country.GetTag() == value_.substr(2, value_.size());
   }

   if (std::holds_alternative<CountryScope>(context.root))
   {
      return std::get_if<CountryScope>(&context.root)->country.GetTag() == value_;
   }

   return false;
}


std::vector<Scope> RootTrigger::FindAllValid([[maybe_unused]] const Context& context,
    [[maybe_unused]] const World& world) const
{
   return {};
}


void PrintTo(const RootTrigger& trigger, std::ostream* os)
{
   *os << std::format("RootTrigger: {{ {} }}\n", trigger.value_);
}

}  // namespace hoi4