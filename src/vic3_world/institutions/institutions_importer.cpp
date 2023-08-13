#include "institutions_importer.h"

namespace vic3
{

std::vector<Institution> ImportInstitutions(std::istream& input_stream)
{
   commonItems::parser institution_parser;
   std::vector<Institution> institutions;
   std::string type;
   int investment;
   int country;
   institution_parser.registerKeyword("institution", [&type](std::istream& input_stream) {
      type = commonItems::getString(input_stream);
   });
   institution_parser.registerKeyword("investment", [&investment](std::istream& input_stream) {
      investment = commonItems::getInt(input_stream);
   });
   institution_parser.registerKeyword("country", [&country](std::istream& input_stream) {
      country = commonItems::getInt(input_stream);
   });
   institution_parser.IgnoreUnregisteredItems();

   institutions.emplace_back(Institution(type, investment, country));

}
}  // namespace vic3