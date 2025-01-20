#include "src/hoi4_world/localizations/localizations_converter.h"

#include <external/commonItems/Localization/LocalizationBlock.h>
#include <external/commonItems/Localization/LocalizationDatabase.h>
#include <external/commonItems/Log.h>
#include <external/fmt/include/fmt/format.h>

#include <ranges>

#include "src/hoi4_world/characters/hoi4_characters_converter.h"
#include "src/vic3_world/characters/vic3_character.h"
#include "src/vic3_world/countries/vic3_country.h"



namespace
{


std::optional<std::string> GetOverlordTag(const std::optional<int>& overlord_number,
    const std::map<int, vic3::Country>& vic3_countries)
{
   if (!overlord_number.has_value())
   {
      return std::nullopt;
   }

   if (const auto vic3_overlord_itr = vic3_countries.find(*overlord_number); vic3_overlord_itr != vic3_countries.end())
   {
      return vic3_overlord_itr->second.GetTag();
   }

   return std::nullopt;
}


std::optional<commonItems::LocalizationBlock> GetLocalizationBlock(const std::optional<std::string>& item,
    const commonItems::LocalizationDatabase& vic3_localizations)
{
   if (item.has_value())
   {
      return vic3_localizations.GetLocalizationBlock(*item);
   }

   return std::nullopt;
}


std::optional<commonItems::LocalizationBlock> GetAdjectiveLocalizationBlock(const std::optional<std::string>& item,
    const commonItems::LocalizationDatabase& vic3_localizations)
{
   if (item.has_value())
   {
      return vic3_localizations.GetLocalizationBlock(fmt::format("{}_ADJ", *item));
   }

   return std::nullopt;
}


std::string ModificationFunction(std::string_view placeholder,
    std::string_view base_localization,
    std::string_view modifying_localization)
{
   std::string updated_localization(base_localization);
   if (updated_localization.find(placeholder) == std::string::npos)
   {
      return updated_localization;
   }

   return updated_localization.replace(updated_localization.find(placeholder),
       placeholder.size(),
       modifying_localization);
}


std::string LoweringModificationFunction(std::string_view placeholder,
    std::string_view base_localization,
    std::string_view modifying_localization)
{
   std::string updated_localization(base_localization);
   if (updated_localization.find(placeholder) == std::string::npos)
   {
      return updated_localization;
   }

   std::string lowercase_modifying_localization{modifying_localization};
   std::ranges::transform(lowercase_modifying_localization, lowercase_modifying_localization.begin(), ::tolower);

   return updated_localization.replace(updated_localization.find(placeholder),
       placeholder.size(),
       lowercase_modifying_localization);
}


std::string OverlordModificationFunction(std::string_view base_localization,
    std::string_view modifying_localization,
    const std::string_view language)
{
   std::string language_string(language);
   if (language_string == "braz_por" || language_string == "french" || language_string == "spanish")
   {
      return fmt::format("{} {}", base_localization, modifying_localization);
   }

   return fmt::format("{} {}", modifying_localization, base_localization);
}


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
      const auto vic3_country_itr = vic3_countries.find(vic3_country_number);
      if (vic3_country_itr != vic3_countries.end())
      {
         vic3_country_tag = vic3_country_itr->second.GetTag();
      }
      else
      {
         continue;
      }
      const vic3::Country& vic3_country = vic3_country_itr->second;
      const std::optional<std::string> vic3_overlord_tag = GetOverlordTag(vic3_country.GetOverlord(), vic3_countries);

      const auto& country_localization_block = GetLocalizationBlock(vic3_country_tag, vic3_localizations);
      const auto& adjective_localization_block = GetAdjectiveLocalizationBlock(vic3_country_tag, vic3_localizations);
      const auto& dynamic_name_block = GetLocalizationBlock(vic3_country.GetDynamicName(), vic3_localizations);
      const auto& dynamic_adjective_block =
          GetLocalizationBlock(vic3_country.GetDynamicAdjective(), vic3_localizations);
      const auto& overlord_adjective_block = GetAdjectiveLocalizationBlock(vic3_overlord_tag, vic3_localizations);

      commonItems::LocalizationBlock name_block(hoi4_country, "english");
      if (dynamic_name_block.has_value())
      {
         name_block = *dynamic_name_block;
         if (country_localization_block.has_value())
         {
            name_block.ModifyForEveryLanguage(*country_localization_block,
                [](const std::string& base_localization,
                    const std::string& modifying_localization,
                    [[maybe_unused]] const std::string& language) {
                   return ModificationFunction("[COUNTRY.GetDefinition.GetName]",
                       base_localization,
                       modifying_localization);
                });
            name_block.ModifyForEveryLanguage(*country_localization_block,
                [](const std::string& base_localization,
                    const std::string& modifying_localization,
                    [[maybe_unused]] const std::string& language) {
                   return LoweringModificationFunction("[COUNTRY.GetDefinition.GetName|l]",
                       base_localization,
                       modifying_localization);
                });
            name_block.ModifyForEveryLanguage(*country_localization_block,
                [](const std::string& base_localization,
                    const std::string& modifying_localization,
                    [[maybe_unused]] const std::string& language) {
                   return ModificationFunction("$NAME$", base_localization, modifying_localization);
                });
         }
         if (adjective_localization_block.has_value())
         {
            name_block.ModifyForEveryLanguage(*adjective_localization_block,
                [](const std::string& base_localization,
                    const std::string& modifying_localization,
                    [[maybe_unused]] const std::string& language) {
                   return ModificationFunction("[COUNTRY.GetDefinition.GetAdjective]",
                       base_localization,
                       modifying_localization);
                });
            name_block.ModifyForEveryLanguage(*adjective_localization_block,
                [](const std::string& base_localization,
                    const std::string& modifying_localization,
                    [[maybe_unused]] const std::string& language) {
                   return LoweringModificationFunction("[COUNTRY.GetDefinition.GetAdjective|l]",
                       base_localization,
                       modifying_localization);
                });
            name_block.ModifyForEveryLanguage(*adjective_localization_block,
                [](const std::string& base_localization,
                    const std::string& modifying_localization,
                    [[maybe_unused]] const std::string& language) {
                   return ModificationFunction("$ADJECTIVE$", base_localization, modifying_localization);
                });
         }
      }
      else if (country_localization_block.has_value())
      {
         name_block = *country_localization_block;
      }

      if (vic3_country.GetUseOverlordPrefix() && overlord_adjective_block.has_value())
      {
         name_block.ModifyForEveryLanguage(*overlord_adjective_block,
             [](const std::string& base_localization,
                 const std::string& modifying_localization,
                 const std::string& language) {
                return OverlordModificationFunction(base_localization, modifying_localization, language);
             });
      }

      country_localizations.AddOrModifyLocalizationBlock(hoi4_country, name_block);
      country_localizations.AddOrModifyLocalizationBlock(fmt::format("{}_DEF", hoi4_country), name_block);

      commonItems::LocalizationBlock adjective_block(fmt::format("{}_ADJ", hoi4_country), "english");
      if (dynamic_adjective_block.has_value())
      {
         adjective_block = *dynamic_adjective_block;
         if (country_localization_block.has_value())
         {
            adjective_block.ModifyForEveryLanguage(*country_localization_block,
                [](const std::string& base_localization,
                    const std::string& modifying_localization,
                    [[maybe_unused]] const std::string& language) {
                   return ModificationFunction("[COUNTRY.GetDefinition.GetName]",
                       base_localization,
                       modifying_localization);
                });
            adjective_block.ModifyForEveryLanguage(*country_localization_block,
                [](const std::string& base_localization,
                    const std::string& modifying_localization,
                    [[maybe_unused]] const std::string& language) {
                   return LoweringModificationFunction("[COUNTRY.GetDefinition.GetName|l]",
                       base_localization,
                       modifying_localization);
                });
            adjective_block.ModifyForEveryLanguage(*country_localization_block,
                [](const std::string& base_localization,
                    const std::string& modifying_localization,
                    [[maybe_unused]] const std::string& language) {
                   return ModificationFunction("$NAME$", base_localization, modifying_localization);
                });
         }
         if (adjective_localization_block.has_value())
         {
            adjective_block.ModifyForEveryLanguage(*adjective_localization_block,
                [](const std::string& base_localization,
                    const std::string& modifying_localization,
                    [[maybe_unused]] const std::string& language) {
                   return ModificationFunction("[COUNTRY.GetDefinition.GetAdjective]",
                       base_localization,
                       modifying_localization);
                });
            adjective_block.ModifyForEveryLanguage(*adjective_localization_block,
                [](const std::string& base_localization,
                    const std::string& modifying_localization,
                    [[maybe_unused]] const std::string& language) {
                   return LoweringModificationFunction("[COUNTRY.GetDefinition.GetAdjective|l]",
                       base_localization,
                       modifying_localization);
                });
            adjective_block.ModifyForEveryLanguage(*adjective_localization_block,
                [](const std::string& base_localization,
                    const std::string& modifying_localization,
                    [[maybe_unused]] const std::string& language) {
                   return ModificationFunction("$ADJECTIVE$", base_localization, modifying_localization);
                });
         }
      }
      else if (adjective_localization_block.has_value())
      {
         adjective_block = *adjective_localization_block;
      }

      if (vic3_country.GetUseOverlordPrefix() && overlord_adjective_block.has_value())
      {
         adjective_block.ModifyForEveryLanguage(*overlord_adjective_block,
             [](const std::string& base_localization,
                 const std::string& modifying_localization,
                 const std::string& language) {
                return OverlordModificationFunction(base_localization, modifying_localization, language);
             });
      }

      country_localizations.AddOrModifyLocalizationBlock(fmt::format("{}_ADJ", hoi4_country), adjective_block);
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

      name_localization_block.ModifyForEveryLanguage([&first_key, &last_key]([[maybe_unused]] const std::string& unused,
                                                         [[maybe_unused]] const std::string& language) {
         return fmt::format("${}$ ${}$", first_key, last_key);
      });
      character_localizations.AddOrModifyLocalizationBlock(name_localization_block.GetKey(), name_localization_block);
   }
   return character_localizations;
}


commonItems::LocalizationDatabase ConvertIdeaLocalizations(const commonItems::LocalizationDatabase& vic3_localizations,
    const std::map<std::string, hoi4::Country>& hoi4_countries,
    const std::map<int, hoi4::Character>& hoi4_characters)
{
   commonItems::LocalizationDatabase idea_localizations("english",
       {"braz_por", "french", "german", "japanese", "polish", "russian", "spanish"});

#pragma warning(push)
#pragma warning(disable : 4566)
   commonItems::LocalizationBlock male_localization_block("king", "english");
   male_localization_block.ModifyLocalization("english", "King {}");
   male_localization_block.ModifyLocalization("braz_por", "Rei {}");
   male_localization_block.ModifyLocalization("french", "Roi {}");
   male_localization_block.ModifyLocalization("german", "König {}");
   male_localization_block.ModifyLocalization("japanese", "{} 王");
   male_localization_block.ModifyLocalization("polish", "Król {}");
   male_localization_block.ModifyLocalization("russian", "Король {}");
   male_localization_block.ModifyLocalization("spanish", "Rey {}");

   commonItems::LocalizationBlock female_localization_block("queen", "english");
   female_localization_block.ModifyLocalization("english", "Queen {}");
   female_localization_block.ModifyLocalization("braz_por", "Rainha {}");
   female_localization_block.ModifyLocalization("french", "Reine {}");
   female_localization_block.ModifyLocalization("german", "Königin {}");
   female_localization_block.ModifyLocalization("japanese", "{} 女王");
   female_localization_block.ModifyLocalization("polish", "Królowa {}");
   female_localization_block.ModifyLocalization("russian", "Королева {}");
   female_localization_block.ModifyLocalization("spanish", "Reina {}");

   commonItems::LocalizationBlock male_localization_description_block("king", "english");
   male_localization_description_block.ModifyLocalization("english",
       "Rallying around the King of [{}.GetName] and the [{}.GetAdjective] Dominions, the [{}.GetAdjective] people "
       "stand united and proud of their imperial legacy.");
   male_localization_description_block.ModifyLocalization("braz_por",
       "Reunindo-se em torno do Rei da [{}.GetName] e dos Domínios [{}.GetAdjective], o povo [{}.GetAdjective] se "
       "mantém unido e orgulhoso de seu legado imperial.");
   male_localization_description_block.ModifyLocalization("french",
       "Rassemblant autour du Roi de [{}.GetName] et des colonies [{}.GetAdjective]s, la nation [{}.GetAdjective] est "
       "unie et fière de son héritage impérial.");
   male_localization_description_block.ModifyLocalization("german",
       "Das [{}.GetAdjective]e Volk versammelt sich um den König von [{}.GetName] und die [{}.GetAdjective]e Kolonien "
       "und ist vereint und stolz auf sein kaiserliches Erbe.");
   male_localization_description_block.ModifyLocalization("japanese",
       "[{}.GetName]の国王と[{}.GetAdjective]の領土の周りに結集し、[{}.GetAdjective]"
       "人は団結し、帝国の遺産を誇りに思っています");
   male_localization_description_block.ModifyLocalization("polish",
       "Gromadząc się wokół Króla [{}.GetName] i [{}.GetAdjective]ich Dominiów, [{}.GetAdjective] są zjednoczeni i "
       "dumni ze swojego imperialnego dziedzictwa.");
   male_localization_description_block.ModifyLocalization("russian",
       "Сплотившись вокруг Короля [{}.GetName] и [{}.GetAdjective]их Доминионов, [{}.GetAdjective]ий народ "
       "объединяется и гордится своим имперским наследием.");
   male_localization_description_block.ModifyLocalization("spanish",
       "Reunida en torno al Rey de [{}.GetName] y las tierras [{}.GetAdjective]s, la nación [{}.GetAdjective] está "
       "unida y orgullosa de su legado imperial.");

   commonItems::LocalizationBlock female_localization_description_block("queen", "english");
   female_localization_description_block.ModifyLocalization("english",
       "Rallying around the Queen of [{}.GetName] and the [{}.GetAdjective] Dominions, the [{}.GetAdjective] people "
       "stand united and proud of their imperial legacy.");
   female_localization_description_block.ModifyLocalization("braz_por",
       "Reunindo-se em torno da Rainha da [{}.GetName] e dos Domínios [{}.GetAdjective], o povo [{}.GetAdjective] se "
       "mantém unido e orgulhoso de seu legado imperial.");
   female_localization_description_block.ModifyLocalization("french",
       "Rassemblant autour de la Reine de [{}.GetName] et des colonies [{}.GetAdjective]s, la nation [{}.GetAdjective] "
       "est unie et fière de son héritage impérial.");
   female_localization_description_block.ModifyLocalization("german",
       "Das [{}.GetAdjective]e Volk versammelt sich um die Königin von [{}.GetName] und die [{}.GetAdjective]e "
       "Kolonien und ist vereint und stolz auf sein kaiserliches Erbe.");
   female_localization_description_block.ModifyLocalization("japanese",
       "[{}.GetName]の女王と[{}.GetAdjective]の領土の周りに結集し、[{}.GetAdjective]"
       "人は団結し、帝国の遺産を誇りに思っています");
   female_localization_description_block.ModifyLocalization("polish",
       "Gromadząc się wokół Królowej [{}.GetName] i [{}.GetAdjective]ich Dominiów, [{}.GetAdjective] są zjednoczeni i "
       "dumni ze swojego imperialnego dziedzictwa.");
   female_localization_description_block.ModifyLocalization("russian",
       "Сплотившись вокруг Королевы [{}.GetName] и [{}.GetAdjective]их Доминионов, [{}.GetAdjective]ий народ "
       "объединяется и гордится своим имперским наследием.");
   female_localization_description_block.ModifyLocalization("spanish",
       "Reunida en torno a la Reina de [{}.GetName] y las tierras [{}.GetAdjective]s, la nación [{}.GetAdjective] está "
       "unida y orgullosa de su legado imperial.");
#pragma warning(pop)

   for (const auto& [tag, country]: hoi4_countries)
   {
      const std::optional<int64_t>& monarch_idea_id = country.GetMonarchIdeaIds();
      if (!monarch_idea_id.has_value())
      {
         continue;
      }

      const auto monarch_itr = hoi4_characters.find(static_cast<int>(*monarch_idea_id));
      if (monarch_itr == hoi4_characters.end())
      {
         continue;
      }

      const std::string monarch_idea_name = hoi4::GetMonarchIdeaName(tag, monarch_itr->second);
      commonItems::LocalizationBlock name_localization_block(monarch_idea_name, "english");
      if (!monarch_itr->second.IsFemale())
      {
         name_localization_block.CopyFrom(male_localization_block);
      }
      else
      {
         name_localization_block.CopyFrom(female_localization_block);
      }

      const std::optional<commonItems::LocalizationBlock>& first_name =
          vic3_localizations.GetLocalizationBlock(monarch_itr->second.GetFirstName());
      if (!first_name.has_value())
      {
         continue;
      }

      const std::optional<commonItems::LocalizationBlock>& last_name =
          vic3_localizations.GetLocalizationBlock(monarch_itr->second.GetLastName());
      if (!last_name.has_value())
      {
         continue;
      }
      commonItems::LocalizationBlock combined_name(*first_name);

      combined_name.ModifyForEveryLanguage(*last_name,
          [](const std::string& base_localization,
              const std::string& modifying_localization,
              [[maybe_unused]] const std::string& language) {
             return fmt::format("{} {}", base_localization, modifying_localization);
          });

      name_localization_block.ModifyForEveryLanguage(combined_name,
          [](const std::string& base_localization,
              const std::string& modifying_localization,
              [[maybe_unused]] const std::string& language) {
             return fmt::vformat(base_localization, fmt::make_format_args(modifying_localization));
          });
      idea_localizations.AddOrModifyLocalizationBlock(name_localization_block.GetKey(), name_localization_block);

      const std::string monarch_idea_description = fmt::format("{}_desc", monarch_idea_name);
      commonItems::LocalizationBlock description_localization_block(monarch_idea_description, "english");
      if (!monarch_itr->second.IsFemale())
      {
         description_localization_block.CopyFrom(male_localization_description_block);
      }
      else
      {
         description_localization_block.CopyFrom(female_localization_description_block);
      }
      description_localization_block.ModifyForEveryLanguage(
          [tag](const std::string& base_localization, [[maybe_unused]] const std::string& language) {
             return fmt::vformat(base_localization, fmt::make_format_args(tag, tag, tag));
          });
      idea_localizations.AddOrModifyLocalizationBlock(description_localization_block.GetKey(),
          description_localization_block);
   }

   return idea_localizations;
}

}  // namespace


hoi4::Localizations hoi4::ConvertLocalizations(const commonItems::LocalizationDatabase& vic3_localizations,
    const std::map<int, std::string>& country_mappings,
    const std::map<std::string, std::string>& hoi4_state_names_to_vic3_state_names,
    const std::map<std::string, vic3::StateRegion>& vic3_state_regions,
    const mappers::ProvinceMapper& province_mapper,
    const std::map<int, vic3::Country>& vic3_countries,
    const std::map<int, vic3::Character>& vic3_characters,
    const std::map<std::string, hoi4::Country>& hoi4_countries,
    const std::map<int, hoi4::Character>& hoi4_characters)
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
   commonItems::LocalizationDatabase idea_localizations =
       ConvertIdeaLocalizations(vic3_localizations, hoi4_countries, hoi4_characters);

   return {country_localizations,
       state_localizations,
       victory_point_localizations,
       character_localizations,
       idea_localizations};
}