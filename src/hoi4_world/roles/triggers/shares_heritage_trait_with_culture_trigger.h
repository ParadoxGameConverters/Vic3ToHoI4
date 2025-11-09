#pragma once



#include <string>

#include "src/hoi4_world/roles/triggers/trigger_base.h"
#include "src/support/polymorphic_equality.h"



namespace hoi4
{

class SharesHeritageTraitWithCultureTrigger: public Trigger
{
  public:
   explicit SharesHeritageTraitWithCultureTrigger(std::string_view value): value_(value) {}
   SharesHeritageTraitWithCultureTrigger(const SharesHeritageTraitWithCultureTrigger&) = default;
   SharesHeritageTraitWithCultureTrigger& operator=(const SharesHeritageTraitWithCultureTrigger&) = default;
   SharesHeritageTraitWithCultureTrigger(SharesHeritageTraitWithCultureTrigger&&) = default;
   SharesHeritageTraitWithCultureTrigger& operator=(SharesHeritageTraitWithCultureTrigger&&) = default;
   ~SharesHeritageTraitWithCultureTrigger() override = default;

   [[nodiscard]] bool operator==(const Trigger& rhs) const override { return PolymorphicEquality(*this, rhs); }
   [[nodiscard]] bool operator==([[maybe_unused]] const SharesHeritageTraitWithCultureTrigger& rhs) const
   {
      return value_ == rhs.value_;
   }
   [[nodiscard]] bool operator<(const Trigger& rhs) const override { return PolymorphicLess(*this, rhs); }
   [[nodiscard]] bool operator<([[maybe_unused]] const SharesHeritageTraitWithCultureTrigger& rhs) const
   {
      return value_ < rhs.value_;
   }

   [[nodiscard]] std::unique_ptr<Trigger> Copy() const override
   {
      return std::make_unique<SharesHeritageTraitWithCultureTrigger>(value_);
   }
   [[nodiscard]] bool IsValid(const Context& context, const World& world) const override;
   [[nodiscard]] std::vector<Scope> FindAllValid([[maybe_unused]] const Context& context,
       [[maybe_unused]] const World& world) const override
   {
      return {};
   }

   // This allows the Google test framework to print human-readable RepeatFocuses if a test fails.
   friend void PrintTo(const SharesHeritageTraitWithCultureTrigger& trigger, std::ostream* os);

  private:
   std::string value_;
};

}  // namespace hoi4