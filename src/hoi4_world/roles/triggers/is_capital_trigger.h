#pragma once



#include <string>

#include "src/hoi4_world/roles/triggers/trigger_base.h"
#include "src/support/polymorphic_equality.h"



namespace hoi4
{

class IsCapitalTrigger: public Trigger
{
  public:
   explicit IsCapitalTrigger(bool value): value_(value) {}
   IsCapitalTrigger(const IsCapitalTrigger&) = default;
   IsCapitalTrigger& operator=(const IsCapitalTrigger&) = default;
   IsCapitalTrigger(IsCapitalTrigger&&) = default;
   IsCapitalTrigger& operator=(IsCapitalTrigger&&) = default;
   ~IsCapitalTrigger() override = default;

   [[nodiscard]] bool operator==(const Trigger& rhs) const override { return PolymorphicEquality(*this, rhs); }
   [[nodiscard]] bool operator==([[maybe_unused]] const IsCapitalTrigger& rhs) const { return value_ == rhs.value_; }
   [[nodiscard]] bool operator<(const Trigger& rhs) const override { return PolymorphicLess(*this, rhs); }
   [[nodiscard]] bool operator<([[maybe_unused]] const IsCapitalTrigger& rhs) const { return value_ < rhs.value_; }

   [[nodiscard]] std::unique_ptr<Trigger> Copy() const override { return std::make_unique<IsCapitalTrigger>(value_); }
   [[nodiscard]] bool IsValid(const Context& context, const World& world) const override;

   // This allows the Google test framework to print human-readable RepeatFocuses if a test fails.
   friend void PrintTo(const IsCapitalTrigger& trigger, std::ostream* os);

  private:
   bool value_;
};

}  // namespace hoi4