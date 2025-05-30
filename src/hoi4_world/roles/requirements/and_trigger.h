#pragma once



#include "src/hoi4_world/roles/requirements/trigger_base.h"



namespace hoi4
{

class AndTrigger: public Trigger
{
  public:
   explicit AndTrigger(std::vector<std::unique_ptr<Trigger>> children): children_(std::move(children)) {}
   ~AndTrigger() override = default;

   [[nodiscard]] bool IsValid(const Scope& scope, const World& world) const override;

  private:
   std::vector<std::unique_ptr<Trigger>> children_;
};

}  // namespace hoi4