#pragma once



#include "src/hoi4_world/roles/triggers/trigger_base.h"
#include "src/support/polymorphic_equality.h"



namespace hoi4
{

class AlwaysTrigger: public Trigger
{
  public:
   explicit AlwaysTrigger(bool value): value_(value) {}
   AlwaysTrigger(const AlwaysTrigger&) = default;
   AlwaysTrigger& operator=(const AlwaysTrigger&) = default;
   AlwaysTrigger(AlwaysTrigger&&) = default;
   AlwaysTrigger& operator=(AlwaysTrigger&&) = default;
   ~AlwaysTrigger() override = default;

   [[nodiscard]] bool operator==(const Trigger& rhs) const override { return PolymorphicEquality(*this, rhs); }
   [[nodiscard]] bool operator==(const AlwaysTrigger& rhs) const { return value_ == rhs.value_; }
   [[nodiscard]] bool operator<(const Trigger& rhs) const override { return PolymorphicLess(*this, rhs); }
   [[nodiscard]] bool operator<(const AlwaysTrigger& rhs) const { return value_ < rhs.value_; }

   [[nodiscard]] std::unique_ptr<Trigger> Copy() const override { return std::make_unique<AlwaysTrigger>(value_); }
   [[nodiscard]] bool IsValid([[maybe_unused]] const Context& context,
       [[maybe_unused]] const World& world) const override
   {
      return value_;
   }
   [[nodiscard]] std::vector<Scope> FindAllValid([[maybe_unused]] const Context& context,
       [[maybe_unused]] const World& world) const override
   {
      return {};
   }

   // This allows the Google test framework to print human-readable RepeatFocuses if a test fails.
   friend void PrintTo(const AlwaysTrigger& trigger, std::ostream* os)
   {
      *os << "AlwaysTrigger: " << trigger.value_ << "\n";
   }

  private:
   bool value_;
};

}  // namespace hoi4