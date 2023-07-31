#ifndef SRC_VIC3WORLD_IDEOLOGIES_IDEOLOGYIMPORTER_H
#define SRC_VIC3WORLD_IDEOLOGIES_IDEOLOGYIMPORTER_H



#include "external/commonItems/Parser.h"
#include "src/vic3_world/ideologies/ideology.h"


namespace vic3
{

class IdeologyImporter
{
  public:
   IdeologyImporter();

   Ideology ImportIdeology(std::istream& input_stream);

  private:
   std::map<Law, ApprovalAmount> law_approvals_;

   commonItems::parser ideology_parser_;
   commonItems::parser lawgroup_parser_;
};

}  // namespace vic3



#endif  // SRC_VIC3WORLD_IDEOLOGIES_IDEOLOGYIMPORTER_H