#ifndef SRC_HOI4WORLD_EVENT_OPTION_IMPORTER_H
#define SRC_HOI4WORLD_EVENT_OPTION_IMPORTER_H



#include <external/commonItems/Parser.h>

#include <string>
#include <vector>

#include "src/hoi4_world/events/event_option.h"



namespace hoi4
{

class EventOptionImporter
{
  public:
   EventOptionImporter();
   EventOption ImportOption(std::istream& the_stream);

  private:
   commonItems::parser option_parser_;

   std::string name_;
   std::string trigger_;
   std::string ai_chance_;
   std::vector<std::string> script_blocks_;
   std::string hidden_effect_;
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_EVENT_OPTION_IMPORTER_H