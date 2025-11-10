#pragma once



#include <string>

#include "src/hoi4_world/roles/triggers/trigger_base.h"
#include "src/support/polymorphic_equality.h"



namespace hoi4
{

class IsHomelandOfCountryCulture: public Trigger
{
  public:
   explicit IsHomelandOfCountryCulture(std::string_view country): country_(country) {}
   IsHomelandOfCountryCulture(const IsHomelandOfCountryCulture&) = default;
   IsHomelandOfCountryCulture& operator=(const IsHomelandOfCountryCulture&) = default;
   IsHomelandOfCountryCulture(IsHomelandOfCountryCulture&&) = default;
   IsHomelandOfCountryCulture& operator=(IsHomelandOfCountryCulture&&) = default;
   ~IsHomelandOfCountryCulture() override = default;

   [[nodiscard]] bool operator==(const Trigger& rhs) const override { return PolymorphicEquality(*this, rhs); }
   [[nodiscard]] bool operator==([[maybe_unused]] const IsHomelandOfCountryCulture& rhs) const
   {
      return country_ == rhs.country_;
   }
   [[nodiscard]] bool operator<(const Trigger& rhs) const override { return PolymorphicLess(*this, rhs); }
   [[nodiscard]] bool operator<([[maybe_unused]] const IsHomelandOfCountryCulture& rhs) const
   {
      return country_ < rhs.country_;
   }

   [[nodiscard]] std::unique_ptr<Trigger> Copy() const override
   {
      return std::make_unique<IsHomelandOfCountryCulture>(country_);
   }
   [[nodiscard]] bool IsValid(const Context& context, const World& world) const override;
   [[nodiscard]] std::vector<Scope> FindAllValid([[maybe_unused]] const Context& context,
       [[maybe_unused]] const World& world) const override
   {
      return {};
   }

   // This allows the Google test framework to print human-readable RepeatFocuses if a test fails.
   friend void PrintTo(const IsHomelandOfCountryCulture& trigger, std::ostream* os);

  private:
   std::string country_;
};

}  // namespace hoi4