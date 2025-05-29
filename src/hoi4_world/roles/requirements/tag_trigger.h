#pragma once



#include "src/hoi4_world/roles/requirements/trigger_base.h"



namespace hoi4
{

class TagTrigger: private Trigger
{
  public:
   TagTrigger(std::string_view tag): tag_(tag) {}
   ~TagTrigger() override = default;

   [[nodiscard]] bool IsValid(const Scope& scope, const World& world) const override;

  private:
   std::string tag_;
};

}  // namespace hoi4