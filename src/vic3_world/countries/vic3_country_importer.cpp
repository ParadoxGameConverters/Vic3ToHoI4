#include "src/vic3_world/countries/vic3_country_importer.h"

#include "external/commonItems/CommonRegexes.h"
#include "external/commonItems/ParserHelpers.h"
#include "external/commonItems/StringUtils.h"



vic3::CountryImporter::CountryImporter()
{
   country_parser_.registerKeyword("definition", [this](std::istream& input_stream) {
      tag_ = commonItems::remQuotes(commonItems::getString(input_stream));
   });
   country_parser_.registerKeyword("capital", [this](std::istream& input_stream) {
      capital_ = commonItems::getInt(input_stream);
   });
   country_parser_.registerKeyword("country_type", [this](std::istream& input_stream) {
      country_type_ = commonItems::getString(input_stream);
   });
   country_parser_.IgnoreUnregisteredItems();
}


vic3::Country vic3::CountryImporter::ImportCountry(int number,
    std::istream& input_stream,
    const std::map<std::string, commonItems::Color>& color_definitions)
{
   tag_.clear();
   capital_ = std::nullopt;
   country_type_.clear();

   country_parser_.parseStream(input_stream);

   commonItems::Color color;
   if (const auto color_itr = color_definitions.find(tag_); color_itr != color_definitions.end())
   {
      color = color_itr->second;
   }

   return Country(
       {.number = number, .tag = tag_, .color = color, .capital_state = capital_, .country_type = country_type_});
}