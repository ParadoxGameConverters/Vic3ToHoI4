#include "src/hoi4_world/roles/role_importer.h"

#include <external/commonItems/CommonRegexes.h>
#include <external/commonItems/ParserHelpers.h>



hoi4::RoleImporter::RoleImporter()
{
   decisions_categories_parser_.registerRegex(commonItems::catchallRegex,
       [this](std::string name, std::istream& input) {
          role_options_.decisions_categories.emplace_back(
              decisions_category_importer_.GetDecisionsCategory(name, input));
       });

   decisions_category_parser_.registerRegex(commonItems::catchallRegex, [this](std::string name, std::istream& input) {
      current_decision_category_ = name;
      decisions_parser_.parseStream(input);
   });
   decisions_parser_.registerRegex(commonItems::catchallRegex, [this](std::string name, std::istream& input) {
      const Decision decision = decision_importer_.GetDecision(name, input);
      auto [itr, success] =
          role_options_.decisions_in_categories.emplace(current_decision_category_, std::vector{decision});
      if (!success)
      {
         itr->second.push_back(decision);
      }
   });

   events_parser_.registerRegex(commonItems::catchallRegex, [this](std::string name, std::istream& input) {
      role_options_.events.emplace_back(event_importer_.ImportEvent(name, input));
   });

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
   role_parser_.registerKeyword("shared_focus", [this](std::istream& input) {
      role_options_.shared_focuses.emplace_back(commonItems::getString(input));
   });
   role_parser_.registerKeyword("focus", [this](std::istream& input) {
      role_options_.focuses.emplace_back(focus_importer_.ImportFocus(input));
   });
   role_parser_.registerKeyword("repeat_focus", [this](std::istream& input) {
      role_options_.repeat_focuses.emplace_back(repeat_focus_importer_.ImportRepeatFocus(input));
   });
   role_parser_.registerKeyword("removed_focus", [this](std::istream& input) {
      role_options_.removed_focuses.emplace_back(commonItems::stringOfItem(input).getString());
   });
   role_parser_.registerKeyword("decisions_categories", [this](std::istream& input) {
      decisions_categories_parser_.parseStream(input);
   });
   role_parser_.registerKeyword("decisions", [this](std::istream& input) {
      decisions_category_parser_.parseStream(input);
   });
   role_parser_.registerKeyword("events", [this](std::istream& input) {
      events_parser_.parseStream(input);
   });
}


hoi4::Role hoi4::RoleImporter::ImportRole(std::string_view name, std::istream& input)
{
   role_options_ = {};
   current_decision_category_.clear();

   role_options_.name = name;
   role_parser_.parseStream(input);

   return Role{role_options_};
}