#include "src/vic3_world/countries/vic3_country_importer.h"

#include "external/commonItems/CommonRegexes.h"
#include "external/commonItems/ParserHelpers.h"
#include "external/commonItems/StringUtils.h"



vic3::CountryImporter::CountryImporter()
{
   country_parser_.registerKeyword("definition", [this](std::istream& input_stream) {
      tag_ = commonItems::remQuotes(commonItems::getString(input_stream));
   });
   country_parser_.IgnoreUnregisteredItems();
}


vic3::Country vic3::CountryImporter::ImportCountry(std::istream& input_stream)
{
   tag_.clear();
   country_parser_.parseStream(input_stream);
   return Country({.tag = tag_});
}