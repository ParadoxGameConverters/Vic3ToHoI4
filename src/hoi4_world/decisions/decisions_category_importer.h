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

   [[nodiscard]] DecisionsCategory GetDecisionsCategory(std::istream& the_stream);

  private:
   commonItems::parser parser;

   std::string name;
   std::string icon;
   std::string picture;
   std::optional<int> priority;
   std::string allowed;
   std::string visible;
   std::string visibilityType;
   std::vector<std::pair<std::string, std::string>> extraItems;
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_DECISIONS_DECISIONSCATEGORYIMPORTER_H