#include "src/vic3_world/ideologies/ideologies_importer.h"

#include <external/commonItems/CommonRegexes.h>



vic3::IdeologiesImporter::IdeologiesImporter()
{
   ideologies_parser_.registerRegex(commonItems::catchallRegex,
       [this](const std::string& ideology_name, std::istream& input_stream) {
          const Ideology ideology = ideology_importer_.ImportIdeology(input_stream);
          ideologies_.emplace(ideology_name, ideology);
       });
}



vic3::Ideologies vic3::IdeologiesImporter::ImportIdeologies(const commonItems::ModFilesystem& mod_filesystem)
{
   ideologies_.clear();

   for (const auto& filename: mod_filesystem.GetAllFilesInFolder("common/ideologies"))
   {
      ideologies_parser_.parseFile(filename);
   }

   return Ideologies(ideologies_);
}