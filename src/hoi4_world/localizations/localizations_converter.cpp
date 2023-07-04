#include "src/hoi4_world/localizations/localizations_converter.h"

#include <ranges>

#include "external/commonItems/Localization/LocalizationBlock.h"
#include "external/commonItems/Localization/LocalizationDatabase.h"
#include "external/fmt/include/fmt/format.h"
#include "src/vic3_world/characters/vic3_character.h"
#include "src/vic3_world/countries/vic3_country.h"



namespace
{

commonItems::LocalizationDatabase ConvertCountryLocalizations(
    const commonItems::LocalizationDatabase& vic3_localizations,
    const std::map<int, std::string>& country_mappings,
    const std::map<int, vic3::Country>& vic3_countries)
{
   commonItems::LocalizationDatabase country_localizations("english",
       {"braz_por", "french", "german", "japanese", "polish", "russian", "spanish"});

   for (const auto& [vic3_country_number, hoi4_country]: country_mappings)
   {
      std::string vic3_country_tag;
      if (const auto vic3_country = vic3_countries.find(vic3_country_number); vic3_country != vic3_countries.end())
      {
         vic3_country_tag = vic3_country->second.GetTag();
      }
      else
      {
         continue;
      }

      const auto& country_localization_block = vic3_localizations.GetLocalizationBlock(vic3_country_tag);
      if (!country_localization_block)
      {
         continue;
      }

      country_localizations.AddOrModifyLocalizationBlock(hoi4_country, *country_localization_block);
      country_localizations.AddOrModifyLocalizationBlock(fmt::format("{}_DEF", hoi4_country),
          *country_localization_block);

      const auto& adjective_localization_block =
          vic3_localizations.GetLocalizationBlock(fmt::format("{}_ADJ", vic3_country_tag));
      if (!adjective_localization_block)
      {
         continue;
      }
      country_localizations.AddOrModifyLocalizationBlock(fmt::format("{}_ADJ", hoi4_country),
          *adjective_localization_block);
   }

   return country_localizations;
}


commonItems::LocalizationDatabase ConvertStateLocalizations(const commonItems::LocalizationDatabase& vic3_localizations,
    const std::map<std::string, std::string>& hoi4_state_names_to_vic3_state_names)
{
   commonItems::LocalizationDatabase state_localizations("english",
       {"braz_por", "french", "german", "japanese", "polish", "russian", "spanish"});

   for (const auto& [hoi4_state_name, vic3_state_name]: hoi4_state_names_to_vic3_state_names)
   {
      const auto& state_localization_block = vic3_localizations.GetLocalizationBlock(vic3_state_name);
      if (!state_localization_block)
      {
         continue;
      }

      state_localizations.AddOrModifyLocalizationBlock(hoi4_state_name, *state_localization_block);
   }

   return state_localizations;
}



commonItems::LocalizationDatabase ConvertVictoryPointLocalizations(
    const commonItems::LocalizationDatabase& vic3_localizations,
    const std::map<std::string, vic3::StateRegion>& vic3_state_regions,
    const mappers::ProvinceMapper& province_mapper)
{
   commonItems::LocalizationDatabase victory_point_localizations("english",
       {"braz_por", "french", "german", "japanese", "polish", "russian", "spanish"});

   int skipped = 0;

   std::vector<std::tuple<int, std::string, std::string>> significant_provinces;
   for (const auto& [region_name, state_region]: vic3_state_regions)
   {
      // gather the significant provinces and translate them to hoi4 provinces
      for (const auto& [vic3_province, type]: state_region.GetSignificantProvinces())
      {
         const auto& hoi4_provinces = province_mapper.GetVic3ToHoi4ProvinceMapping(vic3_province);
         if (hoi4_provinces.empty())
         {
            ++skipped;
            continue;
         }

         significant_provinces.emplace_back(*hoi4_provinces.begin(), type, region_name);
      }
   }

   // some might share a province, so put the more significant ones later to override the less significant ones
   std::ranges::sort(significant_provinces,
       [](const std::tuple<int, std::string, std::string>& a, const std::tuple<int, std::string, std::string>& b) {
          const std::string& a_type = std::get<1>(a);
          const std::string& b_type = std::get<1>(b);
          if (a_type == b_type)
          {
             return std::get<0>(a) > std::get<0>(b);
          }
          if (a_type == "city")
          {
             return false;
          }
          if (b_type == "city")
          {
             return true;
          }
          if (a_type == "port")
          {
             return false;
          }
          if (b_type == "port")
          {
             return true;
          }
          return a_type > b_type;
       });

   // apply the localizations
   for (const auto& [province, type, region_name]: significant_provinces)
   {
      const auto& vic3_localization =
          vic3_localizations.GetLocalizationBlock(fmt::format("HUB_NAME_{}_{}", region_name, type));
      if (!vic3_localization.has_value())
      {
         ++skipped;
         continue;
      }

      victory_point_localizations.AddOrModifyLocalizationBlock(fmt::format("VICTORY_POINTS_{}", province),
          *vic3_localization);
   }

   Log(LogLevel::Info) << skipped << " VP localizations were skipped.";

   return victory_point_localizations;
}


commonItems::LocalizationDatabase ConvertCharacterLocalizations(
    const commonItems::LocalizationDatabase& vic3_localizations,
    const std::map<int, vic3::Character>& vic3_characters)
{
   commonItems::LocalizationDatabase character_localizations("english",
       {"braz_por", "french", "german", "japanese", "polish", "russian", "spanish"});

   for (const auto& character: vic3_characters | std::views::values)
   {
      const auto& first_name_localization_block = vic3_localizations.GetLocalizationBlock(character.GetFirstName());
      if (!first_name_localization_block)
      {
         continue;
      }
      character_localizations.AddOrModifyLocalizationBlock(character.GetFirstName(), *first_name_localization_block);

      const auto& last_name_localization_block = vic3_localizations.GetLocalizationBlock(character.GetLastName());
      if (!last_name_localization_block)
      {
         continue;
      }
      character_localizations.AddOrModifyLocalizationBlock(character.GetLastName(), *last_name_localization_block);

      const auto& first_key = first_name_localization_block->GetKey();
      const auto& last_key = last_name_localization_block->GetKey();
      commonItems::LocalizationBlock name_localization_block(fmt::format("{}_{}", first_key, last_key),
          *first_name_localization_block);

      name_localization_block.ModifyForEveryLanguage(
          [&first_key, &last_key](const std::string& unused, const std::string& language) {
             return fmt::format("${}$ ${}$", first_key, last_key);
          });
      character_localizations.AddOrModifyLocalizationBlock(name_localization_block.GetKey(), name_localization_block);
   }
   return character_localizations;
}
}  // namespace



hoi4::Localizations hoi4::ConvertLocalizations(const commonItems::LocalizationDatabase& vic3_localizations,
    const std::map<int, std::string>& country_mappings,
    const std::map<std::string, std::string>& hoi4_state_names_to_vic3_state_names,
    const std::map<std::string, vic3::StateRegion>& vic3_state_regions,
    const mappers::ProvinceMapper& province_mapper,
    const std::map<int, vic3::Country>& vic3_countries,
    const std::map<int, vic3::Character>& vic3_characters)
{
   Log(LogLevel::Info) << "\tConverting localizations";
   commonItems::LocalizationDatabase country_localizations =
       ConvertCountryLocalizations(vic3_localizations, country_mappings, vic3_countries);
   commonItems::LocalizationDatabase state_localizations =
       ConvertStateLocalizations(vic3_localizations, hoi4_state_names_to_vic3_state_names);
   commonItems::LocalizationDatabase victory_point_localizations =
       ConvertVictoryPointLocalizations(vic3_localizations, vic3_state_regions, province_mapper);
   commonItems::LocalizationDatabase character_localizations =
       ConvertCharacterLocalizations(vic3_localizations, vic3_characters);

   return {country_localizations, state_localizations, victory_point_localizations, character_localizations};
}