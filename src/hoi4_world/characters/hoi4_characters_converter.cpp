#include "src/hoi4_world/characters/hoi4_characters_converter.h"

#include "external/fmt/include/fmt/format.h"
#include "hoi4_character_converter.h"



namespace
{
int PickUnusedId(const std::map<int, vic3::Character>& source_characters)
{
   while (source_characters.find(hoi4::Character::GetGenId()) != source_characters.end())
   {
      hoi4::Character::IncrementGenId();
   }
   return hoi4::Character::GetGenId();
}

int FindPrimeMinister(const std::map<int, vic3::InterestGroup>& igs, const vic3::Country& source_country)
{
   int prime_minister_id = source_country.GetHeadOfStateId();
   float max_clout = 0.0F;

   for (const auto& ig_id: source_country.GetInterestGroupIds())
   {
      const auto& ig_itr = igs.find(ig_id);
      if (ig_itr == igs.end())
      {
         continue;
      }
      const auto& ig = ig_itr->second;

      if (!ig.IsInGovernment())
      {
         continue;
      }


      if (max_clout < ig.GetClout())
      {
         max_clout = ig.GetClout();
         prime_minister_id = ig.GetLeader();
      }
   }
   return prime_minister_id;
}

std::pair<int, hoi4::Character> ConvertCountryLeader(const std::map<int, vic3::InterestGroup>& igs,
    const std::map<int, vic3::Character>& source_characters,
    const std::string& leader_type,
    const std::string& tag,
    const std::string& country_ideology,
    const vic3::Country& source_country,
    std::map<std::string, mappers::CultureQueue>& culture_queues)
{
   if (leader_type == "council")
   {
      const auto new_council = vic3::Character({
          .id = PickUnusedId(source_characters),
          .first_name = "The",
          .last_name = "Council",
          .culture = *source_country.GetPrimaryCultures().begin(),
          .roles = {"politician"},
      });

      return {new_council.GetId(),
          hoi4::ConvertCharacter(new_council,
              new_council.GetId(),
              leader_type,
              tag,
              country_ideology,
              source_country,
              culture_queues,
              {})};  // country_mapper only needed for spies
   }

   int leader_id;
   if (leader_type == "prime_minister")
   {
      leader_id = FindPrimeMinister(igs, source_country);
   }
   else
   {
      leader_id = source_country.GetHeadOfStateId();
   }

   const auto& source_character_itr = source_characters.find(leader_id);
   if (source_character_itr == source_characters.end())
   {
      Log(LogLevel::Error) << fmt::format("{} country leader has no definition with ID: {}.", tag, leader_id);
      return {0, hoi4::Character({})};
   }
   return {leader_id,
       hoi4::ConvertCharacter(source_character_itr->second,
           leader_id,
           leader_type,
           tag,
           country_ideology,
           source_country,
           culture_queues,
           {})};  // country_mapper only needed for spies
}

std::pair<std::vector<int>, std::set<int>> PickAllOtherCharacters(const int leader_id,
    const std::map<int, vic3::Character>& source_characters,
    const vic3::Country& source_country)
{
   std::vector<int> leader_ids;
   std::set<int> spy_ids;
   leader_ids.push_back(leader_id);  // Starting Country Leader must be first.

   // In the future we may separate this out into PickGenerals, PickAdmirals and such if we wish to add limits.
   // Currently, we just grab any character with valid data.
   for (const auto& character_id: source_country.GetCharacterIds())
   {
      if (leader_id == character_id)
      {
         continue;
      }
      const auto& source_character_itr = source_characters.find(character_id);
      if (source_character_itr == source_characters.end())
      {
         Log(LogLevel::Warning) << fmt::format("Failed to find vic3 character associated with ID: {}", character_id);
         continue;
      }
      const vic3::Character& source_character = source_character_itr->second;
      if (source_character.GetRoles().contains("agitator") && source_character.GetRoles().size() == 1)
      {
         spy_ids.emplace(character_id);
      }
      else
      {
         leader_ids.push_back(character_id);
      }
   }
   return {leader_ids, spy_ids};
}
////////////////////////////////////
////////////////////////////////////
////////////////////////////////////
void AssignPortraitsForCulture()
{
}
}  // namespace


std::pair<std::vector<int>, std::set<int>> hoi4::ConvertCharacters(std::map<int, Character>& characters,
    const std::map<int, vic3::Character>& source_characters,
    const std::string& tag,
    const std::string& country_ideology,
    const vic3::Country& source_country,
    const std::map<int, vic3::InterestGroup>& igs,
    const mappers::LeaderTypeMapper& leader_type_mapper,
    std::map<std::string, mappers::CultureQueue>& culture_queues,
    const mappers::CountryMapper& country_mapper)
{
   // Starting Country Leader must be first.
   const std::string leader_type = leader_type_mapper.GetCountryLeaderType(source_country.GetActiveLaws());
   const auto& [leader_id, leader] =
       ConvertCountryLeader(igs, source_characters, leader_type, tag, country_ideology, source_country, culture_queues);
   characters.emplace(leader_id, leader);

   const auto& [leader_ids, spy_ids] = PickAllOtherCharacters(leader_id, source_characters, source_country);

   std::vector<int> all_ids;
   std::ranges::copy(leader_ids, std::back_inserter(all_ids));
   std::ranges::copy(spy_ids, std::back_inserter(all_ids));

   for (const auto& character_id: all_ids)
   {
      const auto& source_character_itr = source_characters.find(character_id);
      if (source_character_itr == source_characters.end())
      {
         // Should not be possible
         continue;
      }
      characters.emplace(character_id,
          ConvertCharacter(source_character_itr->second,
              leader_id,
              leader_type,
              tag,
              country_ideology,
              source_country,
              culture_queues,
              country_mapper));
   }
   return {leader_ids, spy_ids};
}

void hoi4::AssignPortraits(std::map<int, Character>& characters,
    const std::map<std::string, mappers::CultureQueue>& culture_queues)
{
   std::map<std::string, int> portrait_counts;
}
