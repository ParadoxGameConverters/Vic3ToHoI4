#include "src/hoi4_world/roles/triggers/is_homeland_of_country_cultures.h"



namespace hoi4
{

bool IsHomelandOfCountryCulture::IsValid(const Context& context, [[maybe_unused]] const World& world) const
{
   const StateScope* maybe_state = std::get_if<StateScope>(&context.this_scope);
   if (maybe_state == nullptr)
   {
      return false;
   }

   const auto possible_country = world.GetCountry(country_);
   if (!possible_country.has_value())
   {
      return false;
   }

   return std::ranges::any_of(possible_country->GetPrimaryCultures(),
       [&maybe_state](const std::string& primary_culture) {
          return maybe_state->state.HasHomeland(primary_culture);
       });
}


void PrintTo([[maybe_unused]] const IsHomelandOfCountryCulture& trigger, std::ostream* os)
{
   *os << "IsHomelandOfCountryCulture: " << trigger.country_ << "\n";
}

}  // namespace hoi4