#ifndef SRC_VIC3WORLD_INTEREST_GROUPS_INTERESTGROUPIMPORTER_H
#define SRC_VIC3WORLD_INTEREST_GROUPS_INTERESTGROUPIMPORTER_H



#include <istream>

#include "external/commonItems/Parser.h"
#include "src/vic3_world/interest_groups/interest_group.h"



namespace vic3
{

class InterestGroupImporter
{
  public:
   InterestGroupImporter();

   [[nodiscard]] InterestGroup ImportInterestGroup(int id, std::istream& input_stream);

  private:
   commonItems::parser ig_parser_;

   std::string type_;

   int country_id_ = 0;
   int leader_ = 0;
   float clout_ = 0;
   bool in_government_ = false;
   std::set<std::string> ideologies_;
};

}  // namespace vic3



#endif  // SRC_VIC3WORLD_INTEREST_GROUPS_INTERESTGROUPIMPORTER_H