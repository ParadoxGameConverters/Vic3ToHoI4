#include "src/hoi4_world/roles/role_importer.h"

#include "external/commonItems/ParserHelpers.h"



hoi4::RoleImporter::RoleImporter()
{
   role_parser_.registerKeyword("category", [this](std::istream& input) {
      role_options_.category = commonItems::getString(input);
   });
   role_parser_.registerKeyword("requirements", [this](std::istream& input) {
      role_options_.requirements = commonItems::stringOfItem(input).getString();
   });
   role_parser_.registerKeyword("score", [this](std::istream& input) {
      role_options_.score = static_cast<float>(commonItems::getDouble(input));
   });
   role_parser_.registerKeyword("block_role", [this](std::istream& input) {
      role_options_.blockers.emplace_back(commonItems::getString(input));
   });
   role_parser_.registerKeyword("block_category", [this](std::istream& input) {
      role_options_.blockers.emplace_back(commonItems::getString(input));
   });
   role_parser_.registerKeyword("focus", [this](std::istream& input) {
      role_options_.focuses.emplace_back(commonItems::stringOfItem(input).getString());
   });
   role_parser_.registerKeyword("repeat_focus", [this](std::istream& input) {
      role_options_.repeat_focuses.emplace_back(commonItems::stringOfItem(input).getString());
   });
   role_parser_.registerKeyword("removed_focus", [this](std::istream& input) {
      role_options_.removed_focuses.emplace_back(commonItems::stringOfItem(input).getString());
   });
   role_parser_.registerKeyword("decision", [this](std::istream& input) {
      role_options_.decisions.emplace_back(commonItems::stringOfItem(input).getString());
   });
   role_parser_.registerKeyword("event", [this](std::istream& input) {
      role_options_.events.emplace_back(commonItems::stringOfItem(input).getString());
   });
}


hoi4::Role hoi4::RoleImporter::ImportRole(std::string_view name, std::istream& input)
{
   role_options_ = {};

   role_options_.name = name;
   role_parser_.parseStream(input);

   return Role{role_options_};
}
