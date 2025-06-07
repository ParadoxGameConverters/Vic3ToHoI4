#pragma once



#include "src/hoi4_world/roles/requirements/trigger_base.h"
#include "src/support/polymorphic_equality.h"



namespace hoi4
{

class AndTrigger: public Trigger
{
  public:
   explicit AndTrigger(std::vector<std::unique_ptr<Trigger>> children): children_(std::move(children)) {}
   ~AndTrigger() override = default;

   [[nodiscard]] bool operator==(const Trigger& rhs) const override { return PolymorphicEquality(*this, rhs); }
   [[nodiscard]] bool operator==(const AndTrigger& rhs) const;
   [[nodiscard]] bool operator<(const Trigger& rhs) const override { return PolymorphicLess(*this, rhs); }
   [[nodiscard]] bool operator<(const AndTrigger& rhs) const;

   [[nodiscard]] bool IsValid(const Scope& scope, const World& world) const override;

  private:
   std::vector<std::unique_ptr<Trigger>> children_;
};

}  // namespace hoi4