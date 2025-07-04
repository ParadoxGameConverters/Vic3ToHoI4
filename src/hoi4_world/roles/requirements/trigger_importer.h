#pragma once



#include <memory>

#include "external/commonItems/Parser.h"
#include "src/hoi4_world/roles/requirements/trigger_base.h"



namespace hoi4
{

class TriggerImporter
{
  public:
   TriggerImporter();
   std::unique_ptr<Trigger> ImportTrigger(std::istream& input);

  public:
   std::vector<std::unique_ptr<Trigger>> ImportTriggers(std::istream& input);

   commonItems::parser trigger_parser_;

   std::vector<std::unique_ptr<Trigger>> triggers_;
};

}  // namespace hoi4