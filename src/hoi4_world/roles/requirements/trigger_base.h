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

   [[nodiscard]] virtual bool operator==(const Trigger& rhs) const = 0;
   [[nodiscard]] virtual bool operator<(const Trigger& rhs) const = 0;

   [[nodiscard]] virtual bool IsValid(const Scope& scope, const World& world) const = 0;
};

}  // namespace hoi4