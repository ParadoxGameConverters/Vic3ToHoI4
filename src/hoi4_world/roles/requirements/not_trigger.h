#pragma once



#include "src/hoi4_world/roles/requirements/trigger_base.h"
#include "src/support/polymorphic_equality.h"



namespace hoi4
{

class NotTrigger: public Trigger
{
  public:
   explicit NotTrigger(std::vector<std::unique_ptr<Trigger>> children): children_(std::move(children)) {}
   NotTrigger(const NotTrigger&) = default;
   NotTrigger& operator=(const NotTrigger&) = default;
   NotTrigger(NotTrigger&&) = default;
   NotTrigger& operator=(NotTrigger&&) = default;
   ~NotTrigger() override = default;

   [[nodiscard]] bool operator==(const Trigger& rhs) const override { return PolymorphicEquality(*this, rhs); }
   [[nodiscard]] bool operator==(const NotTrigger& rhs) const;
   [[nodiscard]] bool operator<(const Trigger& rhs) const override { return PolymorphicLess(*this, rhs); }
   [[nodiscard]] bool operator<(const NotTrigger& rhs) const;

   [[nodiscard]] std::unique_ptr<Trigger> Copy() const override;
   [[nodiscard]] bool IsValid(const Scope& scope, const World& world) const override;

  private:
   std::vector<std::unique_ptr<Trigger>> children_;
};

}  // namespace hoi4