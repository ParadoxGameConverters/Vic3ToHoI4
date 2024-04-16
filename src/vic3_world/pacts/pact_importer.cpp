#include "src/vic3_world/pacts/pact_importer.h"

#include "external/commonItems/ParserHelpers.h"
#include "external/fmt/include/fmt/format.h"



vic3::PactImporter::PactImporter()
{
   targets_parser_.registerKeyword("first", [this](std::istream& input_stream) {
      first_id_ = commonItems::getInt(input_stream);
   });
   targets_parser_.registerKeyword("second", [this](std::istream& input_stream) {
      second_id_ = commonItems::getInt(input_stream);
   });
   targets_parser_.IgnoreUnregisteredItems();

   pact_parser_.registerKeyword("targets", [this](std::istream& input_stream) {
      targets_parser_.parseStream(input_stream);
   });
   pact_parser_.registerKeyword("first", [this](std::istream& input_stream) {
      first_id_ = commonItems::getInt(input_stream);
   });
   pact_parser_.registerKeyword("second", [this](std::istream& input_stream) {
      second_id_ = commonItems::getInt(input_stream);
   });
   pact_parser_.registerKeyword("action", [this](std::istream& input_stream) {
      action_ = commonItems::getString(input_stream);
   });
   pact_parser_.registerKeyword("start_date", [this](std::istream& input_stream) {
      start_date_ = date(commonItems::getString(input_stream));
   });
   pact_parser_.registerKeyword("forced_duration", [this](std::istream& input_stream) {
      forced_duration_ = commonItems::getInt(input_stream);
   });
   pact_parser_.IgnoreUnregisteredItems();
}


vic3::Pact vic3::PactImporter::ImportPact(const int id, std::istream& input_stream)
{
   first_id_ = 0;
   second_id_ = 0;
   action_.clear();
   start_date_ = date();
   forced_duration_.reset();

   pact_parser_.parseStream(input_stream);

   return {first_id_, second_id_, action_, start_date_, forced_duration_};
}