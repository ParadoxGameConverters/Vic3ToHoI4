#include "src/mappers/provinces/province_mapping_importer.h"

#include "external/commonItems/CommonRegexes.h"
#include "external/commonItems/ParserHelpers.h"



mappers::ProvinceMappingImporter::ProvinceMappingImporter()
{
   parser_.registerKeyword("vic3", [this](std::istream& theStream) {
      auto input = commonItems::getString(theStream);
      if (input.starts_with("0x"))
      {
         input = input.substr(1, input.length());
      }
      vic3_provinces_.push_back(input);
   });
   parser_.registerKeyword("hoi4", [this](std::istream& theStream) {
      hoi4_provinces_.push_back(commonItems::getInt(theStream));
   });
   parser_.registerKeyword("comment", commonItems::ignoreItem);
   parser_.IgnoreAndLogUnregisteredItems();
}


mappers::ProvinceMapping mappers::ProvinceMappingImporter::ImportProvinceMapping(std::istream& input_stream)
{
   vic3_provinces_.clear();
   hoi4_provinces_.clear();

   parser_.parseStream(input_stream);

   return ProvinceMapping{vic3_provinces_, hoi4_provinces_};
}