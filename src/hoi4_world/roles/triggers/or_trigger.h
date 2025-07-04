#pragma once



#include "src/hoi4_world/roles/triggers/trigger_base.h"
#include "src/support/polymorphic_equality.h"



namespace hoi4
{

class OrTrigger: public Trigger
{
  public:
   explicit OrTrigger(std::vector<std::unique_ptr<Trigger>> children): children_(std::move(children)) {}
   OrTrigger(const OrTrigger&) = default;
   OrTrigger& operator=(const OrTrigger&) = default;
   OrTrigger(OrTrigger&&) = default;
   OrTrigger& operator=(OrTrigger&&) = default;

   ~OrTrigger() override = default;

   [[nodiscard]] bool operator==(const Trigger& rhs) const override { return PolymorphicEquality(*this, rhs); }
   [[nodiscard]] bool operator==(const OrTrigger& rhs) const;
   [[nodiscard]] bool operator<(const Trigger& rhs) const override { return PolymorphicLess(*this, rhs); }
   [[nodiscard]] bool operator<(const OrTrigger& rhs) const;

   [[nodiscard]] std::unique_ptr<Trigger> Copy() const override;
   [[nodiscard]] bool IsValid(const Scope& scope, const World& world) const override;

   // This allows the Google test framework to print human-readable RepeatFocuses if a test fails.
   friend void PrintTo(const OrTrigger& trigger, std::ostream* os);

  private:
   std::vector<std::unique_ptr<Trigger>> children_;
};

}  // namespace hoi4