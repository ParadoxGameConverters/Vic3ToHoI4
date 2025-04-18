#include "src/hoi4_world/events/event_option_importer.h"

#include "external/commonItems/CommonRegexes.h"
#include "external/commonItems/ParserHelpers.h"



namespace hoi4
{

EventOptionImporter::EventOptionImporter()
{
   option_parser_.registerKeyword("name", [this](std::istream& the_stream) {
      name_ = commonItems::getString(the_stream);
   });
   option_parser_.registerKeyword("trigger", [this](std::istream& the_stream) {
      const commonItems::stringOfItem script_block_string(the_stream);
      trigger_ = script_block_string.getString();
   });
   option_parser_.registerKeyword("ai_chance", [this](std::istream& the_stream) {
      const commonItems::stringOfItem ai_chance_string(the_stream);
      ai_chance_ = ai_chance_string.getString();
   });
   option_parser_.registerRegex(commonItems::catchallRegex,
       [this](const std::string& block_name, std::istream& the_stream) {
          const commonItems::stringOfItem script_block_string(the_stream);
          const std::string block_string = block_name + " " + script_block_string.getString() + "\n";
          script_blocks_.push_back(block_string);
       });
   option_parser_.registerKeyword("hidden_effect", [this](std::istream& the_stream) {
      const commonItems::stringOfItem hidden_effect_string(the_stream);
      hidden_effect_ = hidden_effect_string.getString();
   });
}


EventOption EventOptionImporter::ImportOption(std::istream& the_stream)
{
   name_.clear();
   trigger_.clear();
   ai_chance_.clear();
   script_blocks_.clear();
   hidden_effect_.clear();

   option_parser_.parseStream(the_stream);

   return EventOption{
       .name = name_,
       .trigger = trigger_,
       .ai_chance = ai_chance_,
       .script_blocks = script_blocks_,
       .hidden_effect = hidden_effect_,
   };
}

}  // namespace hoi4