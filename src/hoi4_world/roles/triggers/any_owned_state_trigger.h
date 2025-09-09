#pragma once



#include "src/hoi4_world/roles/triggers/trigger_base.h"
#include "src/support/polymorphic_equality.h"



namespace hoi4
{

class AnyOwnedStateTrigger: public Trigger
{
  public:
   explicit AnyOwnedStateTrigger(std::vector<std::unique_ptr<Trigger>> children): children_(std::move(children)) {}
   AnyOwnedStateTrigger(const AnyOwnedStateTrigger&) = default;
   AnyOwnedStateTrigger& operator=(const AnyOwnedStateTrigger&) = default;
   AnyOwnedStateTrigger(AnyOwnedStateTrigger&&) = default;
   AnyOwnedStateTrigger& operator=(AnyOwnedStateTrigger&&) = default;
   ~AnyOwnedStateTrigger() override = default;

   [[nodiscard]] bool operator==(const Trigger& rhs) const override { return PolymorphicEquality(*this, rhs); }
   [[nodiscard]] bool operator==(const AnyOwnedStateTrigger& rhs) const;
   [[nodiscard]] bool operator<(const Trigger& rhs) const override { return PolymorphicLess(*this, rhs); }
   [[nodiscard]] bool operator<(const AnyOwnedStateTrigger& rhs) const;

   [[nodiscard]] std::unique_ptr<Trigger> Copy() const override;
   [[nodiscard]] bool IsValid(const Context& context, const World& world) const override;
   [[nodiscard]] std::vector<Scope> FindAllValid(const Context& context, const World& world) const override;

   // This allows the Google test framework to print human-readable RepeatFocuses if a test fails.
   friend void PrintTo(const AnyOwnedStateTrigger& trigger, std::ostream* os);

  private:
   std::vector<std::unique_ptr<Trigger>> children_;
};

}  // namespace hoi4