#pragma once



#include <string>

#include "src/hoi4_world/roles/triggers/trigger_base.h"
#include "src/support/polymorphic_equality.h"



namespace hoi4
{

class TagTrigger: public Trigger
{
  public:
   explicit TagTrigger(std::string_view tag): tag_(tag) {}
   TagTrigger(const TagTrigger&) = default;
   TagTrigger& operator=(const TagTrigger&) = default;
   TagTrigger(TagTrigger&&) = default;
   TagTrigger& operator=(TagTrigger&&) = default;
   ~TagTrigger() override = default;

   [[nodiscard]] bool operator==(const Trigger& rhs) const override { return PolymorphicEquality(*this, rhs); }
   [[nodiscard]] bool operator==(const TagTrigger& rhs) const { return tag_ == rhs.tag_; }
   [[nodiscard]] bool operator<(const Trigger& rhs) const override { return PolymorphicLess(*this, rhs); }
   [[nodiscard]] bool operator<(const TagTrigger& rhs) const { return tag_ < rhs.tag_; }

   [[nodiscard]] std::unique_ptr<Trigger> Copy() const override;
   [[nodiscard]] bool IsValid(const Scope& scope, const World& world) const override;

   // This allows the Google test framework to print human-readable RepeatFocuses if a test fails.
   friend void PrintTo(const TagTrigger& trigger, std::ostream* os);

  private:
   std::string tag_;
};

}  // namespace hoi4