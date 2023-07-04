#include "src/hoi4_world/characters/hoi4_character_converter.h"



namespace
{
std::optional<hoi4::Admiral> GeneratePossibleAdmiral(const vic3::Character& source_character)
{
   if (!source_character.GetRoles().contains("admiral"))
   {
      return std::nullopt;
   }

   // Trait/skill mapper later
   return hoi4::Admiral();
}
std::optional<hoi4::General> GeneratePossibleGeneral(const vic3::Character& source_character)
{
   if (!source_character.GetRoles().contains("general"))
   {
      return std::nullopt;
   }

   // Trait/skill mapper later
   return hoi4::General();
}
std::optional<hoi4::Advisor> GeneratePossibleAdvisor(const vic3::Character& source_character,
    const std::optional<hoi4::Leader>& is_leader)
{
   if (!source_character.GetRoles().contains("politician") || is_leader)
   {
      return std::nullopt;
   }

   // dummies for now
   return hoi4::Advisor(.slot = "political_advisor");
}
std::optional<hoi4::Leader> GeneratePossibleLeader(const vic3::Character& source_character,
    const mappers::LeaderTypeMapper& leader_type_mapper,
    const vic3::Country& source_country,
    const hoi4::Country& country,
    const int prime_minister_id)
{
   const std::string leader_type = leader_type_mapper.GetCountryLeaderType(source_country.GetActiveLaws());
   if (leader_type == "council")
   {
      return std::nullopt;
   }

   if (leader_type == "head_of_state" && source_character.GetId() == source_country.GetHeadOfStateId())
   {
      return hoi4::Leader({.sub_ideology = country.GetSubIdeology()});
   }
   if (leader_type == "prime_minister" && prime_minister_id)
   {
      return hoi4::Leader({.sub_ideology = country.GetSubIdeology()});
   }
   return std::nullopt;
}
std::optional<hoi4::Spy> GeneratePossibleSpy(const vic3::Character& source_character,
    const hoi4::Country& country,
    const mappers::CountryMapper& country_mapper)
{
   if (!source_character.GetRoles().contains("agitator") || source_character.GetRoles().size() > 1)
   {
      return std::nullopt;
   }

   std::set<std::string> nationalities;
   nationalities.emplace(country.GetTag());
   if (const auto& possible_id = source_character.GetOriginCountryId())
   {
      if (const auto& possible_tag = country_mapper.GetHoiTag(*possible_id))
      {
         nationalities.emplace(*possible_tag);
      }
   }

   // Trait/skill mapper later
   return hoi4::Spy({.nationalities = nationalities});
}


}  // namespace

std::optional<hoi4::Character> hoi4::ConvertCharacter(const vic3::Character& source_character,
    int prime_minister,
    const Country& country,
    const vic3::Country& source_country,
    const mappers::LeaderTypeMapper& leader_type_mapper,
    const mappers::CountryMapper& country_mapper)
{
   std::optional<Admiral> admiral_data = GeneratePossibleAdmiral(source_character);
   std::optional<General> general_data = GeneratePossibleGeneral(source_character);
   std::optional<Leader> leader_data =
       GeneratePossibleLeader(source_character, leader_type_mapper, source_country, country, prime_minister);
   std::optional<Advisor> advisor_data = GeneratePossibleAdvisor(source_character, leader_data);
   std::optional<Spy> spy_data = GeneratePossibleSpy(source_character, country, country_mapper);

   std::string portrait_alias;

   return Character({.id = source_character.GetId(),
       .first_name = source_character.GetFirstName(),
       .last_name = source_character.GetLastName(),
       .portrait_alias = portrait_alias,
       .is_female = source_character.IsFemale(),
       .admiral_data = admiral_data,
       .general_data = general_data,
       .leader_data = leader_data,
       .advisor_data = advisor_data,
       .spy_data = spy_data});
}
