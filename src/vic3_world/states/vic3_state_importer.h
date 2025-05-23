#ifndef SRC_VIC3WORLD_STATES_VIC3STATEIMPORTER_H
#define SRC_VIC3WORLD_STATES_VIC3STATEIMPORTER_H



#include <external/commonItems/Parser.h>

#include <istream>
#include <set>

#include "src/vic3_world/states/vic3_state.h"



namespace vic3
{

class StateImporter
{
  public:
   StateImporter();

   [[nodiscard]] State ImportState(std::string_view key, std::istream& input_stream);

  private:
   commonItems::parser state_parser_;
   commonItems::parser provinces_parser_;
   commonItems::parser pop_statistics_parser_;

   std::optional<int> owner_number_;
   bool incorporated_ = false;
   float infrastructure_ = 0.0F;
   std::set<int> provinces_;
   int population_ = 0;
   int employed_population_ = 0;
};

}  // namespace vic3



#endif  // SRC_VIC3WORLD_STATES_VIC3STATEIMPORTER_H