#pragma once


#include "src/hoi4_world/roles/requirements/always_trigger.h"
#include "src/hoi4_world/roles/requirements/and_trigger.h"
#include "src/hoi4_world/roles/requirements/not_trigger.h"
#include "src/hoi4_world/roles/requirements/or_trigger.h"
#include "src/hoi4_world/roles/requirements/tag_trigger.h"



namespace hoi4
{

void PrintTo(const Trigger& trigger, std::ostream* os)
{
   if (typeid(trigger) == typeid(AlwaysTrigger))
   {
      PrintTo(dynamic_cast<const AlwaysTrigger&>(trigger), os);
   }
   else if (typeid(trigger) == typeid(AndTrigger))
   {
      PrintTo(dynamic_cast<const AndTrigger&>(trigger), os);
   }
   else if (typeid(trigger) == typeid(NotTrigger))
   {
      PrintTo(dynamic_cast<const NotTrigger&>(trigger), os);
   }
   else if (typeid(trigger) == typeid(OrTrigger))
   {
      PrintTo(dynamic_cast<const OrTrigger&>(trigger), os);
   }
   else if (typeid(trigger) == typeid(TagTrigger))
   {
      PrintTo(dynamic_cast<const TagTrigger&>(trigger), os);
   }
}

}  // namespace hoi4