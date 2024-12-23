#include "src/hoi4_world/characters/hoi4_character_converter.h"

#include <external/commonItems/Log.h>
#include <external/fmt/include/fmt/format.h>



namespace
{
std::optional<hoi4::Admiral> GeneratePossibleAdmiral(const vic3::Character& source_character,
    const std::set<int>& admiral_ids,
    const mappers::CharacterTraitMapper& character_trait_mapper)
{
   if (!source_character.GetRoles().contains("admiral"))
   {
      return std::nullopt;
   }
   if (!admiral_ids.contains(source_character.GetId()))
   {
      return std::nullopt;
   }

   // Trait/skill mapping
   return character_trait_mapper.GetAdmiralMappedData(source_character.GetTraits());
}
std::optional<hoi4::General> GeneratePossibleGeneral(const vic3::Character& source_character,
    const std::set<int>& field_marshal_ids,
    const std::set<int>& general_ids,
    const mappers::CharacterTraitMapper& character_trait_mapper)
{
   if (!source_character.GetRoles().contains("general"))
   {
      return std::nullopt;
   }
   if (!field_marshal_ids.contains(source_character.GetId()) && !general_ids.contains(source_character.GetId()))
   {
      return std::nullopt;
   }

   // Trait/skill mapping
   return character_trait_mapper.GetGeneralMappedData(source_character.GetTraits(),
       field_marshal_ids.contains(source_character.GetId()));
}
std::optional<hoi4::Advisor> GeneratePossibleAdvisor(const vic3::Character& source_character,
    const std::optional<hoi4::Leader>& is_leader,
    const std::set<int>& advisor_ids,
    const mappers::CharacterTraitMapper& character_trait_mapper)
{
   if (!source_character.GetRoles().contains("politician"))
   {
      return std::nullopt;
   }
   if (!advisor_ids.contains(source_character.GetId()))
   {
      return std::nullopt;
   }

   return hoi4::Advisor({
       .traits = character_trait_mapper.GetAdvisorMappedTraits(source_character.GetTraits()),
       .slot = "political_advisor",
   });
}
std::optional<hoi4::Leader> GeneratePossibleLeader(const vic3::Character& source_character,
    const std::string& ideology,
    const int leader_id)
{
   if (source_character.GetId() != leader_id)
   {
      return std::nullopt;
   }

   return hoi4::Leader({.sub_ideology = ideology});
}
std::optional<hoi4::Spy> GeneratePossibleSpy(const vic3::Character& source_character,
    const std::string& tag,
    const mappers::CountryMapper& country_mapper,
    const std::set<int>& spy_ids,
    const mappers::CharacterTraitMapper& character_trait_mapper)
{
   if (!source_character.GetRoles().contains("agitator"))
   {
      return std::nullopt;
   }
   if (!spy_ids.contains(source_character.GetId()))
   {
      return std::nullopt;
   }

   hoi4::Spy spy_data;

   // Add exile nationalities
   spy_data.nationalities.emplace(tag);
   if (const auto& possible_id = source_character.GetOriginCountryId())
   {
      if (const auto& possible_tag = country_mapper.GetHoiTag(*possible_id))
      {
         spy_data.nationalities.emplace(*possible_tag);
      }
   }

   // Trait mapping
   spy_data.traits = character_trait_mapper.GetSpyMappedTraits(source_character.GetTraits());

   return spy_data;
}

void EnqueueCharacterForPortrait(mappers::CultureQueue& culture_queue,
    const vic3::Character& source_character,
    const std::string& ideology,
    const std::set<std::string>& laws,
    const std::string& leader_type,
    const bool is_admiral,
    const bool is_general,
    const bool is_leader,
    const bool is_advisor,
    const bool is_spy)
{
   // Characters can be multiple roles at the same time, only give them a portrait for one role.

   if (source_character.IsFemale())
   {
      if (is_leader && hoi4::HasMonarchs(leader_type, laws))
      {
         culture_queue["monarch_female"].push_back(source_character.GetId());
         return;
      }
      if (is_spy)
      {
         culture_queue["operative_female"].push_back(source_character.GetId());
         return;
      }
      // If you're not a spy or a queen, generic catchall for all other leaders.
      culture_queue["female_leader"].push_back(source_character.GetId());
      return;
   }

   // Being a monarch takes priority over all
   if (is_leader && hoi4::HasMonarchs(leader_type, laws))
   {
      culture_queue["monarch_male"].push_back(source_character.GetId());
      return;
   }

   // Martial portraits take priority over political ones.
   if (is_admiral)
   {
      culture_queue["navy"].push_back(source_character.GetId());
      return;
   }
   if (is_general)
   {
      culture_queue["army"].push_back(source_character.GetId());
      return;
   }
   if (is_leader && leader_type == "council")
   {
      culture_queue["council"].push_back(source_character.GetId());
      return;
   }
   if (is_leader)
   {
      if (ideology == "communism")
      {
         culture_queue["leader_communism"].push_back(source_character.GetId());
         return;
      }
      if (ideology == "democratic")
      {
         culture_queue["leader_democratic"].push_back(source_character.GetId());
         return;
      }
      if (ideology == "fascism")
      {
         culture_queue["leader_fascism"].push_back(source_character.GetId());
         return;
      }
      culture_queue["leader_neutrality"].push_back(source_character.GetId());
      return;
   }
   if (is_advisor)
   {
      if (ideology == "communism")
      {
         culture_queue["advisor_communism"].push_back(source_character.GetId());
         return;
      }
      if (ideology == "democratic")
      {
         culture_queue["advisor_democratic"].push_back(source_character.GetId());
         return;
      }
      if (ideology == "fascism")
      {
         culture_queue["advisor_fascism"].push_back(source_character.GetId());
         return;
      }
      culture_queue["advisor_neutrality"].push_back(source_character.GetId());
      return;
   }
   if (is_spy)
   {
      culture_queue["operative_male"].push_back(source_character.GetId());
      return;
   }
   Log(LogLevel::Warning) << fmt::format("Unable to find a portrait category for character with ID: {}. {} {}.",
       source_character.GetId(),
       source_character.GetFirstName(),
       source_character.GetLastName());
}

mappers::CultureQueue& PrepareCultureQueue(const std::string& culture,
    std::map<std::string, mappers::CultureQueue>& culture_queues)
{
   culture_queues.emplace(culture, mappers::CultureQueue{});
   return culture_queues.at(culture);
}
}  // namespace

bool hoi4::HasMonarchs(const std::string& leader_type, const std::set<std::string>& laws)
{
   return leader_type == "head_of_state" && laws.contains("law_monarchy");
}

hoi4::Character hoi4::ConvertCharacter(const vic3::Character& source_character,
    int leader_id,
    const RoleIds& role_ids,
    const std::string& leader_type,
    const std::string& tag,
    const std::string& country_ideology,
    const std::string& sub_ideology,
    const std::set<std::string>& laws,
    const mappers::CountryMapper& country_mapper,
    const mappers::CharacterTraitMapper& character_trait_mapper,
    std::map<std::string, mappers::CultureQueue>& culture_queues)
{
   std::optional<Admiral> admiral_data =
       GeneratePossibleAdmiral(source_character, role_ids.admiral_ids, character_trait_mapper);
   std::optional<General> general_data = GeneratePossibleGeneral(source_character,
       role_ids.field_marshal_ids,
       role_ids.general_ids,
       character_trait_mapper);
   std::optional<Leader> leader_data = GeneratePossibleLeader(source_character, sub_ideology, leader_id);
   std::optional<Advisor> advisor_data =
       GeneratePossibleAdvisor(source_character, leader_data, role_ids.advisor_ids, character_trait_mapper);
   std::optional<Spy> spy_data =
       GeneratePossibleSpy(source_character, tag, country_mapper, role_ids.spy_ids, character_trait_mapper);

   mappers::CultureQueue& culture_queue = PrepareCultureQueue(source_character.GetCulture(), culture_queues);
   EnqueueCharacterForPortrait(culture_queue,
       source_character,
       country_ideology,
       laws,
       leader_type,
       admiral_data.has_value(),
       general_data.has_value(),
       leader_data.has_value(),
       advisor_data.has_value(),
       spy_data.has_value());

   return Character({.id = source_character.GetId(),
       .first_name = source_character.GetFirstName(),
       .last_name = source_character.GetLastName(),
       .is_female = source_character.IsFemale(),
       .admiral_data = admiral_data,
       .general_data = general_data,
       .leader_data = leader_data,
       .advisor_data = advisor_data,
       .spy_data = spy_data});
}
