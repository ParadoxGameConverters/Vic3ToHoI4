#include "src/mappers/technology/tech_mapping_importer.h"

#include "external/commonItems/ParserHelpers.h"



mappers::TechMappingImporter::TechMappingImporter()
{
   parser.registerKeyword("vic3", [this](std::istream& theStream) {
      vic3_requirements_.insert(commonItems::singleString{theStream}.getString());
   });
   parser.registerKeyword("limit", [this](std::istream& theStream) {
      limit_ = commonItems::singleString{theStream}.getString();
   });
   parser.registerKeyword("hoi4", [this](std::istream& theStream) {
      techs_.insert(commonItems::singleString{theStream}.getString());
   });
}


mappers::TechMapping mappers::TechMappingImporter::ImportTechMapping(std::istream& input_stream)
{
   vic3_requirements_.clear();
   limit_.reset();
   techs_.clear();

   parser.parseStream(input_stream);

   return TechMapping{vic3_requirements_, limit_, techs_};
}