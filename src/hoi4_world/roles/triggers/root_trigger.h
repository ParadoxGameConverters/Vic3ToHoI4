#pragma once



#include "src/hoi4_world/roles/triggers/trigger_base.h"
#include "src/support/polymorphic_equality.h"



namespace hoi4
{

class RootTrigger: public Trigger
{
  public:
   explicit RootTrigger(const std::string_view value): value_(value) {}
   RootTrigger(const RootTrigger&) = default;
   RootTrigger& operator=(const RootTrigger&) = default;
   RootTrigger(RootTrigger&&) = default;
   RootTrigger& operator=(RootTrigger&&) = default;
   ~RootTrigger() override = default;

   [[nodiscard]] bool operator==(const Trigger& rhs) const override { return PolymorphicEquality(*this, rhs); }
   [[nodiscard]] bool operator==(const RootTrigger& rhs) const { return value_ == rhs.value_; }
   [[nodiscard]] bool operator<(const Trigger& rhs) const override { return PolymorphicLess(*this, rhs); }
   [[nodiscard]] bool operator<(const RootTrigger& rhs) const { return value_ < rhs.value_; }

   [[nodiscard]] std::unique_ptr<Trigger> Copy() const override { return std::make_unique<RootTrigger>(value_); }
   [[nodiscard]] bool IsValid([[maybe_unused]] const Context& context,
       [[maybe_unused]] const World& world) const override;
   [[nodiscard]] std::vector<Scope> FindAllValid([[maybe_unused]] const Context& context,
       [[maybe_unused]] const World& world) const override;

   // This allows the Google test framework to print human-readable RepeatFocuses if a test fails.
   friend void PrintTo(const RootTrigger& trigger, std::ostream* os);

  private:
   std::string value_;
};

}  // namespace hoi4