#ifndef SRC_HOI4WORLD_EVENT_IMPORTER_H
#define SRC_HOI4WORLD_EVENT_IMPORTER_H



#include <external/commonItems/Parser.h>

#include <optional>
#include <string>
#include <vector>

#include "src/hoi4_world/events/event.h"
#include "src/hoi4_world/events/event_option.h"
#include "src/hoi4_world/events/event_option_importer.h"



namespace hoi4
{

class EventImporter
{
  public:
   EventImporter();
   Event ImportEvent(std::string_view type, std::istream& input);

  private:
   commonItems::parser event_parser_;
   EventOptionImporter option_importer_;

   std::string id_;
   std::optional<std::string> title_;
   std::vector<std::string> descriptions_;
   std::optional<std::string> picture_;
   bool major_event_ = false;
   bool triggered_only_ = false;
   std::optional<bool> hidden_;
   std::string trigger_;
   std::optional<bool> fire_only_once_;
   std::string mean_time_to_happen_;
   std::string immediate_;
   std::vector<EventOption> options_;
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_EVENT_IMPORTER_H