#ifndef SRC_VIC3WORLD_IDEOLOGIES_IDEOLOGIESIMPORTER_H
#define SRC_VIC3WORLD_IDEOLOGIES_IDEOLOGIESIMPORTER_H



#include <external/commonItems/ModLoader/ModFilesystem.h>
#include <external/commonItems/Parser.h>

#include "src/vic3_world/ideologies/ideologies.h"
#include "src/vic3_world/ideologies/ideology_importer.h"


namespace vic3
{

class IdeologiesImporter
{
  public:
   IdeologiesImporter();

   Ideologies ImportIdeologies(const commonItems::ModFilesystem& mod_filesystem);

  private:
   std::map<std::string, Ideology> ideologies_;

   IdeologyImporter ideology_importer_;
   commonItems::parser ideologies_parser_;
};

}  // namespace vic3



#endif  // SRC_VIC3WORLD_IDEOLOGIES_IDEOLOGIESIMPORTER_H