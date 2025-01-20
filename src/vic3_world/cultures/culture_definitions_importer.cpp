#include "src/vic3_world/cultures/culture_definitions_importer.h"

#include <external/commonItems/CommonRegexes.h>
#include <external/commonItems/ParserHelpers.h>
#include <external/commonItems/StringUtils.h>

std::map<std::string, vic3::CultureDefinition> vic3::ImportCultureDefinitions(
    const commonItems::ModFilesystem& mod_filesystem)
{
   std::map<std::string, vic3::CultureDefinition> culture_definitions;

   vic3::NameList name_list;
   std::set<std::string> traits;
   std::set<std::string> ethnicities;

   commonItems::parser ethnicityParser;
   ethnicityParser.registerRegex(commonItems::integerRegex,
       [&ethnicities](const std::string& weight, std::istream& input_stream) {
          ethnicities.emplace(commonItems::getString(input_stream));
       });
   ethnicityParser.IgnoreUnregisteredItems();

   commonItems::parser culture_definition_parser;
   culture_definition_parser.registerKeyword("traits", [&traits](std::istream& input_stream) {
      std::vector<std::string> cultural_traits = commonItems::getStrings(input_stream);
      std::ranges::copy(cultural_traits, std::inserter(traits, traits.end()));
   });
   culture_definition_parser.registerKeyword("ethnicities",
       [&ethnicities, &ethnicityParser](std::istream& input_stream) {
          ethnicityParser.parseStream(input_stream);
       });
   culture_definition_parser.registerKeyword("male_common_first_names", [&name_list](std::istream& input_stream) {
      std::vector<std::string> male_names = commonItems::getStrings(input_stream);
      std::ranges::copy(male_names, std::inserter(name_list.male_common_first, name_list.male_common_first.end()));
   });
   culture_definition_parser.registerKeyword("female_common_first_names", [&name_list](std::istream& input_stream) {
      std::vector<std::string> female_names = commonItems::getStrings(input_stream);
      std::ranges::copy(female_names,
          std::inserter(name_list.female_common_first, name_list.female_common_first.end()));
   });
   culture_definition_parser.registerKeyword("common_last_names", [&name_list](std::istream& input_stream) {
      std::vector<std::string> last_names = commonItems::getStrings(input_stream);
      std::ranges::copy(last_names, std::inserter(name_list.common_last, name_list.common_last.end()));
   });
   culture_definition_parser.registerKeyword("noble_last_names", [&name_list](std::istream& input_stream) {
      std::vector<std::string> noble_last_names = commonItems::getStrings(input_stream);
      std::ranges::copy(noble_last_names, std::inserter(name_list.noble_last, name_list.noble_last.end()));
   });
   culture_definition_parser.registerKeyword("male_regal_first_names", [&name_list](std::istream& input_stream) {
      std::vector<std::string> male_noble_names = commonItems::getStrings(input_stream);
      std::ranges::copy(male_noble_names, std::inserter(name_list.male_regal_first, name_list.male_regal_first.end()));
   });
   culture_definition_parser.registerKeyword("female_regal_first_names", [&name_list](std::istream& input_stream) {
      std::vector<std::string> female_noble_names = commonItems::getStrings(input_stream);
      std::ranges::copy(female_noble_names,
          std::inserter(name_list.female_regal_first, name_list.female_regal_first.end()));
   });
   culture_definition_parser.IgnoreUnregisteredItems();

   commonItems::parser culture_definitions_parser;
   culture_definitions_parser.registerRegex(commonItems::catchallRegex,
       [&name_list, &traits, &ethnicities, &culture_definition_parser, &culture_definitions](
           const std::string& culture_name,
           std::istream& input_stream) {
          name_list = vic3::NameList();
          traits.clear();
          ethnicities.clear();

          culture_definition_parser.parseStream(input_stream);
          culture_definitions.emplace(culture_name,
              vic3::CultureDefinition{culture_name, name_list, traits, ethnicities});
       });

   for (const auto& definition_filename: mod_filesystem.GetAllFilesInFolder("common/cultures/"))
   {
      culture_definitions_parser.parseFile(definition_filename);
   }

   return culture_definitions;
}