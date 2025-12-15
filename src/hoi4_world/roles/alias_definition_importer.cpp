#include "src/hoi4_world/roles/alias_definition_importer.h"

#include "external/commonItems/CommonRegexes.h"
#include "external/commonItems/ParserHelpers.h"



namespace hoi4
{

AliasDefinitionImporter::AliasDefinitionImporter()
{
   alias_parser_.registerKeyword("country_flag",
       [this]([[maybe_unused]] const std::string& unused, std::istream& input) {
          country_flag_ = commonItems::getString(input);
       });
   alias_parser_.registerKeyword("alias_identifier",
       [this]([[maybe_unused]] const std::string& unused, std::istream& input) {
          alias_identifier_ = commonItems::getString(input);
       });
   alias_parser_.registerRegex(commonItems::catchallRegex, [](const std::string& unused, std::istream& input) {
      commonItems::ignoreItem(unused, input);
   });
}


AliasDefinition AliasDefinitionImporter::ImportAliasDefinition(std::istream& input)
{
   country_flag_.clear();
   alias_identifier_.clear();

   alias_parser_.parseStream(input);

   return AliasDefinition{.country_flag = country_flag_, .alias_identifier = alias_identifier_};
}

}  // namespace hoi4