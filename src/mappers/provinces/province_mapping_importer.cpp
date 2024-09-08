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
   parser_.registerKeyword("comment", [this](std::istream& the_stream) {
      const std::string raw_string = commonItems::getString(the_stream);
      if (const size_t start = raw_string.find_first_not_of("* "); start != std::string::npos)
      {
          comment_ = raw_string.substr(start, raw_string.length());
          if (const size_t last = comment_.value().find_last_not_of("* "); last != std::string::npos)
          {
              comment_ = comment_.value().substr(0, last + 1);
          }
      }
   });
   parser_.IgnoreAndLogUnregisteredItems();
}


mappers::ProvinceMapping mappers::ProvinceMappingImporter::ImportProvinceMapping(std::istream& input_stream)
{
   vic3_provinces_.clear();
   hoi4_provinces_.clear();
   comment_.reset();

   parser_.parseStream(input_stream);

   return ProvinceMapping{.vic3_provinces = vic3_provinces_, .hoi4_provinces = hoi4_provinces_, .comment = comment_};
}