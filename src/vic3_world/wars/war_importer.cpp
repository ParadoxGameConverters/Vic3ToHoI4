#include "src/vic3_world/wars/war_importer.h"

#include "external/commonItems/ParserHelpers.h"



constexpr int64_t kInvalidWar = 4294967295;



vic3::WarImporter::WarImporter()
{
   war_parser_.registerKeyword("initiator", [this](std::istream& input) {
      initiator_ = commonItems::getInt(input);
   });
   war_parser_.registerKeyword("initiators", [this](std::istream& input) {
      additional_initiators_ = commonItems::getInts(input);
   });
   war_parser_.registerKeyword("target", [this](std::istream& input) {
      target_ = commonItems::getInt(input);
   });
   war_parser_.registerKeyword("targets", [this](std::istream& input) {
      additional_targets_ = commonItems::getInts(input);
   });
   war_parser_.registerKeyword("war", [this](std::istream& input) {
      war_ = commonItems::getLlong(input);
   });
   war_parser_.IgnoreUnregisteredItems();
}


std::optional<vic3::War> vic3::WarImporter::ImportWar(std::istream& input)
{
   initiator_.reset();
   additional_initiators_.clear();
   target_.reset();
   additional_targets_.clear();
   war_.reset();

   war_parser_.parseStream(input);

   if (!war_ || *war_ == kInvalidWar || !initiator_ || !target_)
   {
      return std::nullopt;
   }

   std::set<int> attackers;
   attackers.insert(*initiator_);
   attackers.insert(additional_initiators_.begin(), additional_initiators_.end());
   std::set<int> defenders;
   defenders.insert(*target_);
   defenders.insert(additional_targets_.begin(), additional_targets_.end());

   return vic3::War({
       .original_attacker = *initiator_,
       .attackers = attackers,
       .original_defender = *target_,
       .defenders = defenders,
   });
}