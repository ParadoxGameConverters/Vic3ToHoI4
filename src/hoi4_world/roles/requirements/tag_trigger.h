#pragma once



#include <string>

#include "src/hoi4_world/roles/requirements/trigger_base.h"
#include "src/support/polymorphic_equality.h"



namespace hoi4
{

class TagTrigger: public Trigger
{
  public:
   explicit TagTrigger(std::string_view tag): tag_(tag) {}
   ~TagTrigger() override = default;

   [[nodiscard]] bool operator==(const Trigger& rhs) const override { return PolymorphicEquality(*this, rhs); }
   [[nodiscard]] bool operator==(const TagTrigger& rhs) const { return tag_ == rhs.tag_; }
   [[nodiscard]] bool operator<(const Trigger& rhs) const override { return PolymorphicLess(*this, rhs); }
   [[nodiscard]] bool operator<(const TagTrigger& rhs) const { return tag_ < rhs.tag_; }

   [[nodiscard]] bool IsValid(const Scope& scope, const World& world) const override;

  private:
   std::string tag_;
};

}  // namespace hoi4