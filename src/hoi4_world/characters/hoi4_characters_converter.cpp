#include "src/hoi4_world/characters/hoi4_characters_converter.h"

#include <algorithm>
#include <random>
#include <ranges>

#include "external/fmt/include/fmt/format.h"
#include "hoi4_character_converter.h"


namespace
{
int PickUnusedId(const std::map<int, vic3::Character>& source_characters,
    const std::map<int, hoi4::Character>& characters)
{
   while (source_characters.find(hoi4::Character::GetGenId()) != source_characters.end() ||
          characters.find(hoi4::Character::GetGenId()) != characters.end())
   {
      hoi4::Character::IncrementGenId();
   }
   const int unused_id = hoi4::Character::GetGenId();
   hoi4::Character::IncrementGenId();

   return unused_id;
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
    const std::map<int, hoi4::Character>& characters,
    const std::string& leader_type,
    const std::string& tag,
    const std::string& country_ideology,
    const std::string& sub_ideology,
    const vic3::Country& source_country,
    const mappers::CharacterTraitMapper& character_trait_mapper,
    std::map<std::string, mappers::CultureQueue>& culture_queues)
{
   if (leader_type == "council")
   {
      const auto new_council = vic3::Character({
          .id = PickUnusedId(source_characters, characters),
          .first_name = "The",
          .last_name = "Council",
          .culture = *source_country.GetPrimaryCultures().begin(),
          .roles = {"politician"},
      });

      return {new_council.GetId(),
          hoi4::ConvertCharacter(new_council,
              new_council.GetId(),
              {},  // The council are not Field Marshals
              leader_type,
              tag,
              country_ideology,
              sub_ideology,
              source_country.GetActiveLaws(),
              {},  // country_mapper only needed for spies
              mappers::CharacterTraitMapper({}, {}, {}),
              culture_queues)};
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
           {.field_marshal_ids = {leader_id}},  // Ruler generals are always Field Marshals
           leader_type,
           tag,
           country_ideology,
           sub_ideology,
           source_country.GetActiveLaws(),
           {},  // country_mapper only needed for spies
           character_trait_mapper,
           culture_queues)};
}

using FieldMarshalIds = std::set<int>;
using GeneralIds = std::set<int>;
// Sorts generals between Field Marshals and commanders.
std::pair<FieldMarshalIds, GeneralIds> PickGenerals(const std::map<int, vic3::Character>& source_characters,
    const std::vector<int>& source_character_ids,
    std::set<int>& character_ids)
{
   std::vector<int> ids;
   std::ranges::copy_if(source_character_ids, std::back_inserter(ids), [source_characters](const int id) {
      if (const auto& itr = source_characters.find(id); itr != source_characters.end())
      {
         return itr->second.GetRoles().contains("general");
      }
      return false;
   });

   // 20% of the generals will be Field Marshals, using highest rank first.
   // Remember rank is political and has nothing to do with skill.
   // If limiting the number of generals, prefer higher ranked ones as well.
   std::ranges::sort(ids, [source_characters](const int lhs, const int rhs) {
      return source_characters.at(lhs).GetRank() > source_characters.at(rhs).GetRank();
   });
   const unsigned int num_field_marshals = static_cast<int>(0.2 * static_cast<int>(ids.size()));

   FieldMarshalIds field_marshal_ids;
   GeneralIds general_ids;
   for (unsigned int i = 0; i < ids.size(); ++i)
   {
      character_ids.emplace(ids.at(i));

      if (i < num_field_marshals)
      {
         field_marshal_ids.emplace(ids.at(i));
         continue;
      }
      general_ids.emplace(ids.at(i));
   }
   return {field_marshal_ids, general_ids};
}

using AdmiralIds = std::set<int>;
using AdvisorIds = std::set<int>;
using SpyIds = std::set<int>;
// Naively grabs all remaining characters with valid data
std::tuple<AdmiralIds, AdvisorIds, SpyIds> PickMiscCharacters(const std::map<int, vic3::Character>& source_characters,
    const std::vector<int>& source_character_ids,
    const int leader_id,
    std::set<int>& character_ids)
{
   AdmiralIds admiral_ids;
   AdvisorIds advisor_ids;
   SpyIds spy_ids;

   for (const auto& id: source_character_ids)
   {
      const auto& itr = source_characters.find(id);
      if (itr == source_characters.end())
      {
         continue;
      }

      const auto& roles = itr->second.GetRoles();
      if (roles.contains("admiral"))
      {
         admiral_ids.emplace(id);
         character_ids.emplace(id);
      }
      if (roles.contains("politician") && id != leader_id)
      {
         advisor_ids.emplace(id);
         character_ids.emplace(id);
      }
      if (roles.contains("agitator") && roles.size() == 1)
      {
         spy_ids.emplace(id);
      }
   }
   return {admiral_ids, advisor_ids, spy_ids};
}

std::pair<hoi4::CharacterIds, hoi4::RoleIds> PickCharacters(const int leader_id,
    const std::map<int, vic3::Character>& source_characters,
    const vic3::Country& source_country)
{
   std::set<int> character_set_ids;

   const auto& [field_marshal_ids, general_ids] =
       PickGenerals(source_characters, source_country.GetCharacterIds(), character_set_ids);
   const auto& [admiral_ids, advisor_ids, spy_ids] =
       PickMiscCharacters(source_characters, source_country.GetCharacterIds(), leader_id, character_set_ids);

   hoi4::CharacterIds character_ids;
   character_ids.push_back(leader_id);  // Country leader must be first
   character_set_ids.erase(leader_id);
   std::ranges::copy(character_set_ids, std::back_inserter(character_ids));
   return {character_ids,
       hoi4::RoleIds{
           .leader_id = leader_id,
           .admiral_ids = admiral_ids,
           .field_marshal_ids = field_marshal_ids,
           .general_ids = general_ids,
           .advisor_ids = advisor_ids,
           .spy_ids = spy_ids,
       }};
}

////////////////////////////////////

void PopulatePortraitCounts(const mappers::PortraitPaths& portrait_paths, std::map<std::string, int>& portrait_counts)
{
   for (const auto& portraits: portrait_paths | std::views::values)
   {
      for (const auto& portrait_name: portraits)
      {
         portrait_counts.emplace(portrait_name, 0);
      }
   }
}
void ScramblePortraitPaths(const int playthrough_id, mappers::PortraitPaths& portrait_paths)
{
   // Each vector of the same length will be shuffled the same way.
   // We only want different permutations between playthroughs, within the same playthrough it's the same.
   // Edit the save file? Everything permutes the same.
   // Reconvert? Everything permutes the same.
   // Run a test save in 1930, then the real thing in 1936? everything permutes the same.
   // Start a new run? Everything permutes differently.
   for (auto& portraits: portrait_paths | std::views::values)
   {
      std::ranges::shuffle(portraits, std::default_random_engine(playthrough_id));
   }
}
void ProcessCultureQueue(const mappers::CultureQueue& culture_queue,
    mappers::PortraitPaths& portrait_paths,
    std::map<std::string, int>& portrait_counts,
    std::map<int, hoi4::Character>& characters)
{
   const auto ByPortraitCount = [portrait_counts](const std::string& lhs, const std::string& rhs) {
      return portrait_counts.at(lhs) < portrait_counts.at(rhs);
   };

   for (auto& [key, portraits]: portrait_paths)
   {
      std::ranges::sort(portraits, ByPortraitCount);
      const auto& queued_characters_itr = culture_queue.find(key);
      if (queued_characters_itr == culture_queue.end())
      {
         continue;
      }
      const auto& queued_characters = queued_characters_itr->second;

      for (unsigned int i = 0; i < queued_characters.size() && !portraits.empty(); ++i)
      {
         const std::string& portrait = portraits.at(i % portraits.size());
         portrait_counts.find(portrait)->second++;

         characters.find(queued_characters.at(i))->second.SetPortraitAlias(portrait);
      }
   }
}
mappers::PortraitPaths GetPortraitPaths(const vic3::CultureDefinition& culture,
    const mappers::CultureGraphicsMapper& culture_graphics_mapper,
    const int playthrough_id)
{
   mappers::PortraitPaths portrait_paths = culture_graphics_mapper.MatchCultureToGraphics(culture).portrait_paths;
   ScramblePortraitPaths(playthrough_id, portrait_paths);
   return portrait_paths;
}
}  // namespace


hoi4::Characters hoi4::ConvertCharacters(const std::map<int, vic3::Character>& source_characters,
    const std::string& tag,
    const std::string& country_ideology,
    const std::string& sub_ideology,
    const vic3::Country& source_country,
    const std::map<int, vic3::InterestGroup>& igs,
    const mappers::LeaderTypeMapper& leader_type_mapper,
    const mappers::CharacterTraitMapper& character_trait_mapper,
    const mappers::CountryMapper& country_mapper,
    bool should_have_monarch_idea,
    std::map<int, Character>& characters,
    std::map<std::string, mappers::CultureQueue>& culture_queues)
{
   // Starting Country Leader must be first.
   const std::string leader_type = leader_type_mapper.GetCountryLeaderType(source_country.GetActiveLaws());
   const auto& [leader_id, leader] = ConvertCountryLeader(igs,
       source_characters,
       characters,
       leader_type,
       tag,
       country_ideology,
       sub_ideology,
       source_country,
       character_trait_mapper,
       culture_queues);
   characters.emplace(leader_id, leader);

   const auto& [character_ids, role_ids] = PickCharacters(leader_id, source_characters, source_country);

   std::vector<int> all_ids;
   std::copy(character_ids.begin() + 1, character_ids.end(), std::back_inserter(all_ids));
   std::ranges::copy(role_ids.spy_ids, std::back_inserter(all_ids));
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
              role_ids,
              leader_type,
              tag,
              country_ideology,
              sub_ideology,
              source_country.GetActiveLaws(),
              country_mapper,
              character_trait_mapper,
              culture_queues));
   }

   std::optional<int64_t> monarch_id;
   if (should_have_monarch_idea)
   {
      monarch_id = source_country.GetHeadOfStateId();
   }

   return {character_ids, role_ids.spy_ids, monarch_id};
}

void hoi4::AssignPortraits(const std::map<std::string, mappers::CultureQueue>& culture_queues,
    const mappers::CultureGraphicsMapper& culture_graphics_mapper,
    const std::map<std::string, vic3::CultureDefinition>& source_cultures,
    const int playthrough_id,
    std::map<int, Character>& characters)
{
   std::map<std::string, int> portrait_counts;

   for (const auto& [culture, culture_queue]: culture_queues)
   {
      const auto& culture_itr = source_cultures.find(culture);
      if (culture_itr == source_cultures.end())
      {
         continue;
      }

      auto portrait_paths = GetPortraitPaths(culture_itr->second, culture_graphics_mapper, playthrough_id);
      PopulatePortraitCounts(portrait_paths, portrait_counts);
      ProcessCultureQueue(culture_queue, portrait_paths, portrait_counts, characters);
   }
}


std::string hoi4::GetMonarchIdeaName(std::string_view tag, const hoi4::Character& monarch)
{
   return fmt::format("{}_{}_{}", tag, monarch.GetFirstName(), monarch.GetLastName());
}