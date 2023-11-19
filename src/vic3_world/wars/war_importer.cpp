#include "src/vic3_world/wars/war_importer.h"

#include "external/commonItems/ParserHelpers.h"



constexpr int64_t kInvalidWar = 4294967295;



vic3::WarImporter::WarImporter()
{
   war_parser_.registerKeyword("initiator", [this](std::istream& input) {
      initiator_ = commonItems::getInt(input);
   });
   war_parser_.registerKeyword("target", [this](std::istream& input) {
      target_ = commonItems::getInt(input);
   });
   war_parser_.registerKeyword("involved", [this](std::istream& input) {
      involved_ = commonItems::getInts(input);
   });
   war_parser_.registerKeyword("war", [this](std::istream& input) {
      war_ = commonItems::getLlong(input);
   });
   war_parser_.IgnoreUnregisteredItems();
}


std::optional<vic3::War> vic3::WarImporter::ImportWar(std::istream& input)
{
   initiator_.reset();
   target_.reset();
   involved_.clear();
   war_.reset();

   war_parser_.parseStream(input);

   if (!war_ || *war_ == kInvalidWar || !initiator_ || !target_)
   {
      return std::nullopt;
   }

   std::set<int> attackers;
   std::set<int> defenders;

   auto involved_itr = involved_.begin();
   while (involved_itr != involved_.end() && *involved_itr != *target_)
   {
      attackers.insert(*involved_itr);
      ++involved_itr;
   }
   while (involved_itr != involved_.end())
   {
      defenders.insert(*involved_itr);
      ++involved_itr;
   }

   return vic3::War({.original_attacker = *initiator_,
       .attackers = attackers,
       .original_defender = *target_,
       .defenders = defenders});
}