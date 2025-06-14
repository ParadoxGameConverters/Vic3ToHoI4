#include "src/hoi4_world/events/event_importer.h"

#include <external/commonItems/CommonRegexes.h>
#include <external/commonItems/ParserHelpers.h>



namespace hoi4
{

EventImporter::EventImporter()
{
   event_parser_.registerKeyword("id", [this](std::istream& the_stream) {
      id_ = commonItems::getString(the_stream);
   });
   event_parser_.registerKeyword("title", [this](std::istream& the_stream) {
      title_ = commonItems::getString(the_stream);
   });
   event_parser_.registerKeyword("desc", [this](std::istream& the_stream) {
      descriptions_.push_back(commonItems::stringOfItem{the_stream}.getString());
   });
   event_parser_.registerKeyword("picture", [this](std::istream& the_stream) {
      picture_ = commonItems::getString(the_stream);
   });
   event_parser_.registerKeyword("major", [this](std::istream& the_stream) {
      major_event_ = commonItems::getString(the_stream) == "yes";
   });
   event_parser_.registerKeyword("is_triggered_only", [this](std::istream& the_stream) {
      triggered_only_ = commonItems::getString(the_stream) == "yes";
   });
   event_parser_.registerKeyword("hidden", [this](std::istream& the_stream) {
      hidden_ = commonItems::getString(the_stream) == "yes";
   });
   event_parser_.registerKeyword("trigger", [this](std::istream& the_stream) {
      trigger_ = commonItems::stringOfItem{the_stream}.getString();
   });
   event_parser_.registerKeyword("fire_only_once", [this](std::istream& the_stream) {
      fire_only_once_ = commonItems::getString(the_stream) == "yes";
   });
   event_parser_.registerKeyword("mean_time_to_happen", [this](std::istream& the_stream) {
      mean_time_to_happen_ = commonItems::stringOfItem{the_stream}.getString();
   });
   event_parser_.registerKeyword("immediate", [this](std::istream& the_stream) {
      immediate_ = commonItems::stringOfItem{the_stream}.getString();
   });
   event_parser_.registerKeyword("option", [this](std::istream& the_stream) {
      options_.push_back(option_importer_.ImportOption(the_stream));
   });
   event_parser_.IgnoreUnregisteredItems();
}


Event EventImporter::ImportEvent(std::string_view type, std::istream& input)
{
   id_.clear();
   title_.reset();
   descriptions_.clear();
   picture_.reset();
   major_event_ = false;
   triggered_only_ = false;
   hidden_.reset();
   trigger_.clear();
   fire_only_once_.reset();
   mean_time_to_happen_.clear();
   immediate_.clear();
   options_.clear();

   event_parser_.parseStream(input);

   return {
       .type = std::string(type),
       .id = id_,
       .title = title_,
       .descriptions = descriptions_,
       .picture = picture_,
       .major_event = major_event_,
       .triggered_only = triggered_only_,
       .hidden = hidden_,
       .trigger = trigger_,
       .fire_only_once = fire_only_once_,
       .mean_time_to_happen = mean_time_to_happen_,
       .immediate = immediate_,
       .options = options_,
   };
}

}  // namespace hoi4