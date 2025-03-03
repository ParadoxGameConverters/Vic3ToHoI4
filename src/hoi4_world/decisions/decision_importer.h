#ifndef HOI4_DECISION_IMPORTER_H
#define HOI4_DECISION_IMPORTER_H


#include <external/CommonItems/Parser.h>

#include <optional>
#include <string>

#include "src/hoi4_world/decisions/decision.h"



namespace hoi4
{

class DecisionImporter
{
  public:
   DecisionImporter();

   [[nodiscard]] Decision GetDecision(std::string_view name, std::istream& the_stream);

  private:
   commonItems::parser parser_;

   Decision decision_;
};

}  // namespace hoi4



#endif  // HOI4_DECISION_IMPORTER_H