#ifndef SRC_VIC3WORLD_WARS_WARIMPORTER_H
#define SRC_VIC3WORLD_WARS_WARIMPORTER_H



#include <istream>
#include <optional>

#include "external/commonItems/Parser.h"
#include "src/vic3_world/wars/war.h"



namespace vic3
{

class WarImporter
{
  public:
   WarImporter();
   std::optional<War> ImportWar(std::istream& input);

  private:
   commonItems::parser war_parser_;

   std::optional<int> initiator_;
   std::vector<int> additional_initiators_;
   std::optional<int> target_;
   std::vector<int> additional_targets_;
   std::optional<int64_t> war_;
};

}  // namespace vic3



#endif  //  SRC_VIC3WORLD_WARS_WARIMPORTER_H