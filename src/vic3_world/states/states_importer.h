#ifndef SRC_VIC3WORLD_STATES_STATESIMPORTER
#define SRC_VIC3WORLD_STATES_STATESIMPORTER



#include <istream>
#include <map>

#include "external/commonItems/Parser.h"
#include "src/vic3_world/states/state.h"
#include "src/vic3_world/states/state_importer.h"



namespace vic3
{

class StatesImporter
{
  public:
   StatesImporter();

   [[nodiscard]] std::map<int, State> ImportStates(std::istream& input_stream);

  private:
   commonItems::parser states_parser_;
   commonItems::parser database_parser_;
   StateImporter state_importer_;

   std::map<int, State> states_;
};

}  // namespace vic3



#endif  // SRC_VIC3WORLD_STATES_STATESIMPORTER