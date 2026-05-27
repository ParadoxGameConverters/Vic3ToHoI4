#pragma once



#include <string>

#include "src/hoi4_world/roles/triggers/trigger_base.h"
#include "src/support/polymorphic_equality.h"



namespace hoi4
{

class IsHomelandTrigger: public Trigger
{
  public:
   explicit IsHomelandTrigger(std::string_view culture_name): culture_name_(culture_name) {}
   IsHomelandTrigger(const IsHomelandTrigger&) = default;
   IsHomelandTrigger& operator=(const IsHomelandTrigger&) = default;
   IsHomelandTrigger(IsHomelandTrigger&&) = default;
   IsHomelandTrigger& operator=(IsHomelandTrigger&&) = default;
   ~IsHomelandTrigger() override = default;

   [[nodiscard]] bool operator==(const Trigger& rhs) const override { return PolymorphicEquality(*this, rhs); }
   [[nodiscard]] bool operator==([[maybe_unused]] const IsHomelandTrigger& rhs) const
   {
      return culture_name_ == rhs.culture_name_;
   }
   [[nodiscard]] bool operator<(const Trigger& rhs) const override { return PolymorphicLess(*this, rhs); }
   [[nodiscard]] bool operator<([[maybe_unused]] const IsHomelandTrigger& rhs) const
   {
      return culture_name_ < rhs.culture_name_;
   }

   [[nodiscard]] std::unique_ptr<Trigger> Copy() const override
   {
      return std::make_unique<IsHomelandTrigger>(culture_name_);
   }
   [[nodiscard]] bool IsValid(const Context& context, const World& world) const override;
   [[nodiscard]] std::vector<Scope> FindAllValid([[maybe_unused]] const Context& context,
       [[maybe_unused]] const World& world) const override
   {
      return {};
   }

   // This allows the Google test framework to print human-readable RepeatFocuses if a test fails.
   friend void PrintTo(const IsHomelandTrigger& trigger, std::ostream* os);

  private:
   std::string culture_name_;
};

}  // namespace hoi4