#include "src/hoi4_world/roles/triggers/country_has_primary_culture_trigger.h"



namespace hoi4
{

bool CountryHasPrimaryCultureTrigger::IsValid(const Context& context, [[maybe_unused]] const World& world) const
{
   const CountryScope* maybe_country = std::get_if<CountryScope>(&context.this_scope);
   if (!maybe_country)
   {
      return false;
   }

   if (culture_.starts_with("cu:"))
   {
      return maybe_country->country.GetPrimaryCultures().contains(culture_.substr(3, culture_.length()));
   }
   else
   {
      return maybe_country->country.GetPrimaryCultures().contains(culture_);
   }
}


std::unique_ptr<Trigger> CountryHasPrimaryCultureTrigger::Copy() const
{
   return std::make_unique<CountryHasPrimaryCultureTrigger>(culture_);
}


void PrintTo(const CountryHasPrimaryCultureTrigger& trigger, std::ostream* os)
{
   *os << "CountryHasPrimaryCultureTrigger: " << trigger.culture_ << "\n";
}

}  // namespace hoi4