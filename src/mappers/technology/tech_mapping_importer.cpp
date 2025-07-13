#include "src/mappers/technology/tech_mapping_importer.h"

#include <external/commonItems/ParserHelpers.h>



mappers::TechMappingImporter::TechMappingImporter()
{
   parser_.registerKeyword("vic3", [this](std::istream& the_stream) {
      vic3_requirements_.insert(commonItems::singleString{the_stream}.getString());
   });
   parser_.registerKeyword("limit", [this](std::istream& the_stream) {
      limit_ = commonItems::singleString{the_stream}.getString();
   });
   parser_.registerKeyword("hoi4", [this](std::istream& the_stream) {
      techs_.insert(commonItems::singleString{the_stream}.getString());
   });
}


mappers::TechMapping mappers::TechMappingImporter::ImportTechMapping(std::istream& input_stream)
{
   vic3_requirements_.clear();
   limit_.reset();
   techs_.clear();

   parser_.parseStream(input_stream);

   return TechMapping{vic3_requirements_, limit_, techs_};
}