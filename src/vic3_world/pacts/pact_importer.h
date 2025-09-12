#ifndef SRC_VIC3WORLD_PACTS_PACTIMPORTER_H
#define SRC_VIC3WORLD_PACTS_PACTIMPORTER_H



#include <external/commonItems/Parser.h>

#include <istream>

#include "src/vic3_world/pacts/pact.h"



namespace vic3
{

class PactImporter
{
  public:
   PactImporter();

   [[nodiscard]] Pact ImportPact(std::istream& input_stream);

  private:
   commonItems::parser pact_parser_;
   commonItems::parser targets_parser_;

   PactPartners partners_;
   std::string action_;
   date start_date_;
   std::optional<int> forced_duration_;
};

}  // namespace vic3



#endif  // SRC_VIC3WORLD_PACTS_PACTIMPORTER_H