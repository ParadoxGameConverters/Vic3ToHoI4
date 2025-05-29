#pragma once



#include "src/hoi4_world/roles/requirements/scope.h"
#include "src/hoi4_world/world/hoi4_world.h"



namespace hoi4
{

class Trigger
{
  public:
   Trigger() = default;
   virtual ~Trigger() = default;

   virtual bool IsValid(const Scope& scope, const World& world) const = 0;
};

}  // namespace hoi4