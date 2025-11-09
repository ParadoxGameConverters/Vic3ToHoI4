#include "src/hoi4_world/roles/triggers/shares_heritage_trait_with_culture_trigger.h"



namespace hoi4
{

bool SharesHeritageTraitWithCultureTrigger::IsValid(const Context& context, [[maybe_unused]] const World& world) const
{
   const CultureScope* maybe_culture = std::get_if<CultureScope>(&context.this_scope);
   if (!maybe_culture)
   {
      return false;
   }

   const std::map<std::string, vic3::CultureDefinition>& culture_definitions = world.GetCultureDefinitions();
   auto itr = culture_definitions.find(value_);
   if (itr == culture_definitions.end())
   {
      return false;
   }

   return maybe_culture->culture.GetHeritage() == itr->second.GetHeritage();
}


void PrintTo([[maybe_unused]] const SharesHeritageTraitWithCultureTrigger& trigger, std::ostream* os)
{
   *os << "SharesHeritageTraitWithCultureTrigger: " << trigger.value_ << "\n";
}

}  // namespace hoi4