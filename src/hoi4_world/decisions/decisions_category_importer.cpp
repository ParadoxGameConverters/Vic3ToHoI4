#include "src/hoi4_world/decisions/decisions_category_importer.h"

#include <external/commonItems/CommonRegexes.h>
#include <external/commonItems/ParserHelpers.h>



hoi4::DecisionsCategoryImporter::DecisionsCategoryImporter()
{
   parser_.registerKeyword("name", [this](std::istream& the_stream) {
      name_ = commonItems::getString(the_stream);
   });
   parser_.registerKeyword("icon", [this](std::istream& the_stream) {
      icon_ = commonItems::getString(the_stream);
   });
   parser_.registerKeyword("picture", [this](std::istream& the_stream) {
      picture_ = commonItems::getString(the_stream);
   });
   parser_.registerKeyword("priority", [this](std::istream& the_stream) {
      priority_ = commonItems::getInt(the_stream);
   });
   parser_.registerKeyword("allowed", [this](std::istream& the_stream) {
      allowed_ = commonItems::stringOfItem(the_stream).getString();
   });
   parser_.registerKeyword("visible", [this](std::istream& the_stream) {
      visible_ = commonItems::stringOfItem(the_stream).getString();
   });
   parser_.registerKeyword("visibility_type", [this](std::istream& the_stream) {
      visibility_type_ = commonItems::stringOfItem(the_stream).getString();
   });
   parser_.registerRegex(commonItems::catchallRegex, [this](const std::string& item_name, std::istream& the_stream) {
      extra_items_.push_back(std::make_pair(item_name, commonItems::stringOfItem(the_stream).getString()));
   });
}



hoi4::DecisionsCategory hoi4::DecisionsCategoryImporter::GetDecisionsCategory(std::string_view name,
    std::istream& the_stream)
{
   name_ = name;
   icon_.clear();
   picture_.clear();
   priority_.reset();
   allowed_.clear();
   visible_.clear();
   visibility_type_.clear();
   extra_items_.clear();

   parser_.parseStream(the_stream);

   return {name_, icon_, picture_, priority_, allowed_, visible_, visibility_type_, extra_items_};
}