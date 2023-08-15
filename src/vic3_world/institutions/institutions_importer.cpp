#include "institutions_importer.h"

#include "src/vic3_world/database/database_parser.h"

namespace vic3
{

InstitutionsImporter::InstitutionsImporter(std::map<int, std::vector<Institution>>& out_institutions_map):
    institutions_map_(out_institutions_map)
{
   // create single institution parser
   institution_parser_.registerKeyword("institution", [this](std::istream& input_stream) {
      this->current_institution_.type = commonItems::getString(input_stream);
   });
   institution_parser_.registerKeyword("investment", [this](std::istream& input_stream) {
      this->current_institution_.investment = commonItems::getInt(input_stream);
   });
   institution_parser_.registerKeyword("country", [this](std::istream& input_stream) {
      this->current_institution_.country = commonItems::getInt(input_stream);
   });
   institution_parser_.IgnoreUnregisteredItems();

   const auto institutionParserFunc = [this](std::istream& input_stream) {
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
      this->institution_parser_.parseStream(input_stream);
   };

   parser_ = DatabaseParser(institutionParserFunc);
}

void InstitutionsImporter::operator()(std::istream& input_stream)
{
   parser_.parseStream(input_stream);
   if (this->current_institution_.type != "")
   {
      auto [itr, success] =
          this->institutions_map_.emplace(this->current_institution_.country, std::vector{this->current_institution_});
      if (!success)
      {
         itr->second.emplace_back(current_institution_);
      }
   }
   this->current_institution_ = {};
}

}  // namespace vic3