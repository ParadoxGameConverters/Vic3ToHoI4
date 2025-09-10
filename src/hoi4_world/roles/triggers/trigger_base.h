#pragma once



#include "src/hoi4_world/roles/triggers/context.h"
#include "src/hoi4_world/world/hoi4_world.h"



namespace hoi4
{

class Trigger
{
  public:
   Trigger() = default;
   Trigger(const Trigger&) = default;
   Trigger& operator=(const Trigger&) = default;
   Trigger(Trigger&&) = default;
   Trigger& operator=(Trigger&&) = default;
   virtual ~Trigger() = default;

   [[nodiscard]] virtual bool operator==(const Trigger& rhs) const = 0;
   [[nodiscard]] virtual bool operator<(const Trigger& rhs) const = 0;

   [[nodiscard]] virtual std::unique_ptr<Trigger> Copy() const = 0;
   [[nodiscard]] virtual bool IsValid(const Context& context, const World& world) const = 0;
   [[nodiscard]] virtual std::vector<Scope> FindAllValid(const Context& context, const World& world) const = 0;

   friend void PrintTo(const Trigger& trigger, std::ostream* os);
};

}  // namespace hoi4