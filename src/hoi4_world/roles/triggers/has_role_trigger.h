#pragma once



#include <string>

#include "src/hoi4_world/roles/triggers/trigger_base.h"
#include "src/support/polymorphic_equality.h"



namespace hoi4
{

class HasRoleTrigger: public Trigger
{
  public:
   explicit HasRoleTrigger(std::string_view role): role_(role) {}
   HasRoleTrigger(const HasRoleTrigger&) = default;
   HasRoleTrigger& operator=(const HasRoleTrigger&) = default;
   HasRoleTrigger(HasRoleTrigger&&) = default;
   HasRoleTrigger& operator=(HasRoleTrigger&&) = default;
   ~HasRoleTrigger() override = default;

   [[nodiscard]] bool operator==(const Trigger& rhs) const override { return PolymorphicEquality(*this, rhs); }
   [[nodiscard]] bool operator==(const HasRoleTrigger& rhs) const { return role_ == rhs.role_; }
   [[nodiscard]] bool operator<(const Trigger& rhs) const override { return PolymorphicLess(*this, rhs); }
   [[nodiscard]] bool operator<(const HasRoleTrigger& rhs) const { return role_ < rhs.role_; }

   [[nodiscard]] std::unique_ptr<Trigger> Copy() const override;
   [[nodiscard]] bool IsValid(const Context& context, const World& world) const override;
   [[nodiscard]] std::vector<Scope> FindAllValid([[maybe_unused]] const Context& context,
       [[maybe_unused]] const World& world) const override
   {
      return {};
   }

   // This allows the Google test framework to print human-readable RepeatFocuses if a test fails.
   friend void PrintTo(const HasRoleTrigger& trigger, std::ostream* os);

  private:
   std::string role_;
};

}  // namespace hoi4