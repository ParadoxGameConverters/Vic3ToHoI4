#include "src/hoi4_world/decisions/decisions_category_importer.h"

#include "external/commonItems/CommonRegexes.h"
#include "external/commonItems/ParserHelpers.h"



hoi4::DecisionsCategoryImporter::DecisionsCategoryImporter()
{
   parser.registerKeyword("name", [this](std::istream& the_stream) {
      name = commonItems::getString(the_stream);
   });
   parser.registerKeyword("icon", [this](std::istream& the_stream) {
      icon = commonItems::getString(the_stream);
   });
   parser.registerKeyword("picture", [this](std::istream& the_stream) {
      picture = commonItems::getString(the_stream);
   });
   parser.registerKeyword("priority", [this](std::istream& the_stream) {
      priority = commonItems::getInt(the_stream);
   });
   parser.registerKeyword("allowed", [this](std::istream& the_stream) {
      allowed = commonItems::stringOfItem(the_stream).getString();
   });
   parser.registerKeyword("visible", [this](std::istream& the_stream) {
      visible = commonItems::stringOfItem(the_stream).getString();
   });
   parser.registerKeyword("visibility_type", [this](std::istream& the_stream) {
      visibilityType = commonItems::stringOfItem(the_stream).getString();
   });
   parser.registerRegex(commonItems::catchallRegex, [this](const std::string& itemName, std::istream& the_stream) {
      extraItems.push_back(std::make_pair(itemName, commonItems::stringOfItem(the_stream).getString()));
   });
}



hoi4::DecisionsCategory hoi4::DecisionsCategoryImporter::GetDecisionsCategory(std::istream& the_stream)
{
   name.clear();
   icon.clear();
   picture.clear();
   priority.reset();
   allowed.clear();
   visible.clear();
   visibilityType.clear();
   extraItems.clear();

   parser.parseStream(the_stream);

   return {name, icon, picture, priority, allowed, visible, visibilityType, extraItems};
}