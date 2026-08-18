#pragma once



#include <string>

#include "src/hoi4_world/roles/triggers/trigger_base.h"
#include "src/support/polymorphic_equality.h"



namespace hoi4
{

class CountryHasPrimaryCultureTrigger: public Trigger
{
  public:
   explicit CountryHasPrimaryCultureTrigger(std::string_view culture): culture_(culture) {}
   CountryHasPrimaryCultureTrigger(const CountryHasPrimaryCultureTrigger&) = default;
   CountryHasPrimaryCultureTrigger& operator=(const CountryHasPrimaryCultureTrigger&) = default;
   CountryHasPrimaryCultureTrigger(CountryHasPrimaryCultureTrigger&&) = default;
   CountryHasPrimaryCultureTrigger& operator=(CountryHasPrimaryCultureTrigger&&) = default;
   ~CountryHasPrimaryCultureTrigger() override = default;

   [[nodiscard]] bool operator==(const Trigger& rhs) const override { return PolymorphicEquality(*this, rhs); }
   [[nodiscard]] bool operator==(const CountryHasPrimaryCultureTrigger& rhs) const { return culture_ == rhs.culture_; }
   [[nodiscard]] bool operator<(const Trigger& rhs) const override { return PolymorphicLess(*this, rhs); }
   [[nodiscard]] bool operator<(const CountryHasPrimaryCultureTrigger& rhs) const { return culture_ < rhs.culture_; }

   [[nodiscard]] std::unique_ptr<Trigger> Copy() const override;
   [[nodiscard]] bool IsValid(const Context& context, const World& world) const override;
   [[nodiscard]] std::vector<Scope> FindAllValid([[maybe_unused]] const Context& context,
       [[maybe_unused]] const World& world) const override
   {
      return {};
   }

   // This allows the Google test framework to print human-readable RepeatFocuses if a test fails.
   friend void PrintTo(const CountryHasPrimaryCultureTrigger& trigger, std::ostream* os);

  private:
   std::string culture_;
};

}  // namespace hoi4