#include "institutions_importer.h"

#include <external/commonItems/ParserHelpers.h>

#include "src/vic3_world/database/database_parser.h"



namespace vic3
{

InstitutionsImporter::InstitutionsImporter(std::map<int, std::vector<Institution>>& out_institutions_map):
    institutions_map_(out_institutions_map),
    current_institution_()
{
}

void InstitutionsImporter::operator()(std::istream& input_stream)
{
   // create single institution parser
   // this has to be in the operator() method so this object can be copied/moved and still work
   commonItems::parser institution_parser;
   institution_parser.registerKeyword("institution", [this](std::istream& input_stream) {
      this->current_institution_.type = commonItems::getString(input_stream);
   });
   institution_parser.registerKeyword("investment", [this](std::istream& input_stream) {
      this->current_institution_.investment = commonItems::getInt(input_stream);
   });
   institution_parser.registerKeyword("country", [this](std::istream& input_stream) {
      this->current_institution_.country = commonItems::getInt(input_stream);
   });
   institution_parser.IgnoreUnregisteredItems();

   const auto element_parser = [this, &institution_parser](std::istream& input_stream) {
      institution_parser.parseStream(input_stream);
      if (this->current_institution_.type != "")
      {
         auto [itr, success] = this->institutions_map_.emplace(this->current_institution_.country,
             std::vector{this->current_institution_});
         if (!success)
         {
            itr->second.emplace_back(current_institution_);
         }
      }
      this->current_institution_ = {};
   };

   DatabaseParser database_parser(element_parser);
   database_parser.registerKeyword("dead", commonItems::ignoreItem);
   database_parser.parseStream(input_stream);
}

}  // namespace vic3