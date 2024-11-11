#ifndef SRC_VIC3WORLD_PACTS_PACTIMPORTER_H
#define SRC_VIC3WORLD_PACTS_PACTIMPORTER_H



#include <istream>

#include "external/commonItems/Parser.h"
#include "src/vic3_world/pacts/pact.h"



namespace vic3
{

class PactImporter
{
  public:
   PactImporter();

   [[nodiscard]] Pact ImportPact(int id, std::istream& input_stream);

  private:
   commonItems::parser pact_parser_;
   commonItems::parser targets_parser_;

   int first_id_ = 0;
   int second_id_ = 0;
   std::string action_;
   date start_date_;
   std::optional<int> forced_duration_;
};

}  // namespace vic3



#endif  // SRC_VIC3WORLD_PACTS_PACTIMPORTER_H