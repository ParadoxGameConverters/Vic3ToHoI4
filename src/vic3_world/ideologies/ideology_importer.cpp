#include "src/vic3_world/ideologies/ideology_importer.h"

#include <external/commonItems/CommonRegexes.h>
#include <external/commonItems/ParserHelpers.h>



vic3::IdeologyImporter::IdeologyImporter()
{
   lawgroup_parser_.registerRegex(commonItems::catchallRegex,
       [this](const std::string& law, std::istream& input_stream) {
          int approval = 0;
          const std::string approval_string = commonItems::getString(input_stream);
          if (approval_string == "strongly_disapprove")
          {
             approval = -2;
          }
          if (approval_string == "disapprove")
          {
             approval = -1;
          }
          if (approval_string == "neutral")
          {
             approval = 0;
          }
          if (approval_string == "approve")
          {
             approval = 1;
          }
          if (approval_string == "strongly_approve")
          {
             approval = 2;
          }

          law_approvals_.emplace(law, approval);
       });

   ideology_parser_.registerRegex("lawgroup.*",
       [this]([[maybe_unused]] const std::string& unused, std::istream& input_stream) {
          lawgroup_parser_.parseStream(input_stream);
       });
   ideology_parser_.IgnoreUnregisteredItems();
}


vic3::Ideology vic3::IdeologyImporter::ImportIdeology(std::istream& input_stream)
{
   law_approvals_.clear();

   ideology_parser_.parseStream(input_stream);

   return Ideology(law_approvals_);
}