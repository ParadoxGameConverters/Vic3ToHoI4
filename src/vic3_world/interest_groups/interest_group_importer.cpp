#include "src/vic3_world/interest_groups/interest_group_importer.h"

#include "external/commonItems/ParserHelpers.h"
#include "external/fmt/include/fmt/format.h"

vic3::InterestGroupImporter::InterestGroupImporter()
{
   ig_parser_.registerKeyword("country", [this](std::istream& input_stream) {
      country_id_ = commonItems::getInt(input_stream);
   });
   ig_parser_.registerKeyword("definition", [this](std::istream& input_stream) {
      type_ = commonItems::getString(input_stream);
   });
   ig_parser_.registerKeyword("leader", [this](std::istream& input_stream) {
      leader_ = commonItems::getInt(input_stream);
   });
   ig_parser_.registerKeyword("clout", [this](std::istream& input_stream) {
      clout_ = static_cast<float>(commonItems::getDouble(input_stream));
   });
   ig_parser_.registerKeyword("in_government", [this](std::istream& input_stream) {
      in_government_ = commonItems::getString(input_stream) == "yes";
   });
   ig_parser_.IgnoreUnregisteredItems();
}


vic3::InterestGroup vic3::InterestGroupImporter::ImportInterestGroup(const int id, std::istream& input_stream)
{
   type_.clear();
   country_id_ = 0;
   leader_ = 0;
   clout_ = 0.0F;
   in_government_ = false;

   ig_parser_.parseStream(input_stream);

   return {type_, leader_, clout_, in_government_};
}