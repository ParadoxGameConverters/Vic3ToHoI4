#pragma once



#include "src/hoi4_world/roles/requirements/trigger_base.h"



namespace hoi4
{

class AlwaysTrigger: public Trigger
{
  public:
   explicit AlwaysTrigger(bool value): value_(value) {}
   ~AlwaysTrigger() override = default;

   [[nodiscard]] bool IsValid([[maybe_unused]] const Scope& scope, [[maybe_unused]] const World& world) const override
   {
      return value_;
   }

  private:
   bool value_;
};

}  // namespace hoi4