#include "src/vic3_world/countries/country_definitions_importer.h"

#include "external/commonItems/CommonRegexes.h"
#include "external/commonItems/Parser.h"
#include "external/commonItems/ParserHelpers.h"
#include "external/fmt/include/fmt/format.h"



std::map<std::string, commonItems::Color> vic3::ImportCountryColorDefinitions(
    const commonItems::ModFilesystem& mod_filesystem)
{
   std::map<std::string, commonItems::Color> color_definitions;

   commonItems::parser country_definitions_parser;
   commonItems::parser country_definition_parser;
   commonItems::Color::Factory color_factory;

   std::string current_tag;
   country_definitions_parser.registerRegex(commonItems::catchallRegex,
       [&current_tag, &country_definition_parser](const std::string& tag, std::istream& input_stream) {
          current_tag = tag;
          country_definition_parser.parseStream(input_stream);
       });

   country_definition_parser.registerKeyword("color",
       [color_factory, &current_tag, &color_definitions](std::istream& input_stream) {
          commonItems::Color color = color_factory.getColor(input_stream);
          color_definitions.emplace(current_tag, color);
       });
   country_definition_parser.registerKeyword(commonItems::catchallRegex, commonItems::ignoreItem);

   const auto defintion_filename =
       mod_filesystem.GetActualFileLocation("/game/common/country_definitions/00_countries.txt");
   if (!defintion_filename.has_value())
   {
      throw std::runtime_error("/game/common/country_definitions/00_countries.txt could not be found!");
   }
   country_definitions_parser.parseFile(*defintion_filename);

   return color_definitions;
}