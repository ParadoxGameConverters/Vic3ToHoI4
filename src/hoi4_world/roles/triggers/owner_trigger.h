#pragma once



#include <string>

#include "src/hoi4_world/roles/triggers/trigger_base.h"
#include "src/support/polymorphic_equality.h"



namespace hoi4
{

class OwnerTrigger: public Trigger
{
  public:
   explicit OwnerTrigger(const std::string& owner): owner_(owner) {}
   OwnerTrigger(const OwnerTrigger&) = default;
   OwnerTrigger& operator=(const OwnerTrigger&) = default;
   OwnerTrigger(OwnerTrigger&&) = default;
   OwnerTrigger& operator=(OwnerTrigger&&) = default;
   ~OwnerTrigger() override = default;

   [[nodiscard]] bool operator==(const Trigger& rhs) const override { return PolymorphicEquality(*this, rhs); }
   [[nodiscard]] bool operator==([[maybe_unused]] const OwnerTrigger& rhs) const { return owner_ == rhs.owner_; }
   [[nodiscard]] bool operator<(const Trigger& rhs) const override { return PolymorphicLess(*this, rhs); }
   [[nodiscard]] bool operator<([[maybe_unused]] const OwnerTrigger& rhs) const { return owner_ < rhs.owner_; }

   [[nodiscard]] std::unique_ptr<Trigger> Copy() const override { return std::make_unique<OwnerTrigger>(owner_); }
   [[nodiscard]] bool IsValid(const Context& context, const World& world) const override;
   [[nodiscard]] std::vector<Scope> FindAllValid([[maybe_unused]] const Context& context,
       [[maybe_unused]] const World& world) const override
   {
      return {};
   }

   // This allows the Google test framework to print human-readable RepeatFocuses if a test fails.
   friend void PrintTo(const OwnerTrigger& trigger, std::ostream* os);

  private:
   const std::string owner_;
};

}  // namespace hoi4