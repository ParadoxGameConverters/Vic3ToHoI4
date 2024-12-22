#ifndef SRC_HOI4WORLD_DECISIONS_DECISIONSCATEGORYIMPORTER_H
#define SRC_HOI4WORLD_DECISIONS_DECISIONSCATEGORYIMPORTER_H



#include <istream>

#include "decisions_category.h"
#include "external/commonItems/Parser.h"
#include "src/hoi4_world/decisions/decisions_category.h"



namespace hoi4
{

class DecisionsCategoryImporter
{
  public:
   DecisionsCategoryImporter();

   [[nodiscard]] DecisionsCategory GetDecisionsCategory(std::string_view name, std::istream& the_stream);

  private:
   commonItems::parser parser;

   std::string name_;
   std::string icon_;
   std::string picture_;
   std::optional<int> priority_;
   std::string allowed_;
   std::string visible_;
   std::string visibility_type_;
   std::vector<std::pair<std::string, std::string>> extra_items_;
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_DECISIONS_DECISIONSCATEGORYIMPORTER_H