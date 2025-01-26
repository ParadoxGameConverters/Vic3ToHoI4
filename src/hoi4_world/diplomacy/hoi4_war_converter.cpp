#include "src/hoi4_world/diplomacy/hoi4_war_converter.h"

#include <external/commonItems/Log.h>
#include <external/fmt/include/fmt/format.h>



std::optional<hoi4::War> hoi4::ConvertWar(const vic3::War& source_war,
    const std::set<std::string>& independent_countries,
    const mappers::CountryMapper& country_mapper)
{
   std::string original_defender;
   std::set<std::string> extra_defenders;
   std::string original_attacker;
   std::set<std::string> extra_attackers;

   if (const std::optional<std::string> possible_original_defender =
           country_mapper.GetHoiTag(source_war.GetOriginalDefender());
       possible_original_defender)
   {
      original_defender = *possible_original_defender;
   }
   else
   {
      Log(LogLevel::Warning) << fmt::format("Could not map {}, original defender in a war",
          source_war.GetOriginalDefender());
      return std::nullopt;
   }

   for (const int defender: source_war.GetDefenders())
   {
      if (defender == source_war.GetOriginalDefender())
      {
         continue;
      }
      const std::optional<std::string>& possible_defender = country_mapper.GetHoiTag(defender);
      if (!possible_defender)
      {
         Log(LogLevel::Warning) << fmt::format("Could not map {}, defending in a war", defender);
         continue;
      }
      if (independent_countries.contains(*possible_defender))
      {
         extra_defenders.insert(*possible_defender);
      }
   }

   if (const std::optional<std::string> possible_original_attacker =
           country_mapper.GetHoiTag(source_war.GetOriginalAttacker());
       possible_original_attacker)
   {
      original_attacker = *possible_original_attacker;
   }
   else
   {
      Log(LogLevel::Warning) << fmt::format("Could not map {}, original attacker in a war",
          source_war.GetOriginalAttacker());
      return std::nullopt;
   }

   for (const int attacker: source_war.GetAttackers())
   {
      if (attacker == source_war.GetOriginalAttacker())
      {
         continue;
      }

      const std::optional<std::string> possible_attacker = country_mapper.GetHoiTag(attacker);
      if (!possible_attacker)
      {
         Log(LogLevel::Warning) << fmt::format("Could not map {}, attacking in a war", attacker);
         continue;
      }
      if (independent_countries.contains(*possible_attacker))
      {
         extra_attackers.insert(*possible_attacker);
      }
   }

   return War{
       .original_defender = original_defender,
       .extra_defenders = extra_defenders,
       .original_attacker = original_attacker,
       .extra_attackers = extra_attackers,
   };
}