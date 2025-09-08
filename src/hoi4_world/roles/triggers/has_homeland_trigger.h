#pragma once



#include <string>

#include "src/hoi4_world/roles/triggers/trigger_base.h"
#include "src/support/polymorphic_equality.h"



namespace hoi4
{

class HasHomelandTrigger: public Trigger
{
  public:
   explicit HasHomelandTrigger(std::string_view value): value_(value) {}
   HasHomelandTrigger(const HasHomelandTrigger&) = default;
   HasHomelandTrigger& operator=(const HasHomelandTrigger&) = default;
   HasHomelandTrigger(HasHomelandTrigger&&) = default;
   HasHomelandTrigger& operator=(HasHomelandTrigger&&) = default;
   ~HasHomelandTrigger() override = default;

   [[nodiscard]] bool operator==(const Trigger& rhs) const override { return PolymorphicEquality(*this, rhs); }
   [[nodiscard]] bool operator==([[maybe_unused]] const HasHomelandTrigger& rhs) const { return value_ == rhs.value_; }
   [[nodiscard]] bool operator<(const Trigger& rhs) const override { return PolymorphicLess(*this, rhs); }
   [[nodiscard]] bool operator<([[maybe_unused]] const HasHomelandTrigger& rhs) const { return value_ < rhs.value_; }

   [[nodiscard]] std::unique_ptr<Trigger> Copy() const override { return std::make_unique<HasHomelandTrigger>(value_); }
   [[nodiscard]] bool IsValid(const Context& context, const World& world) const override;
   [[nodiscard]] std::vector<Scope> FindAllValid([[maybe_unused]] const Context& context,
       [[maybe_unused]] const World& world) const override
   {
      return {};
   }

   // This allows the Google test framework to print human-readable RepeatFocuses if a test fails.
   friend void PrintTo(const HasHomelandTrigger& trigger, std::ostream* os);

  private:
   std::string value_;
};

}  // namespace hoi4