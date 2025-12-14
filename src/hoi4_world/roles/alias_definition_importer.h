#pragma once



#include <istream>
#include <string>

#include "external/commonItems/Parser.h"
#include "src/hoi4_world/roles/alias_definition.h"



namespace hoi4
{

class AliasDefinitionImporter
{
  public:
   AliasDefinitionImporter();

   AliasDefinition ImportAliasDefinition(std::istream& input);

  private:
   commonItems::parser alias_parser_;

   std::string country_flag_;
   std::string alias_identifier_;
};

}  // namespace hoi4