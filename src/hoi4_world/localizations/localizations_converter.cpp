#include "src/hoi4_world/localizations/localizations_converter.h"

#include "external/commonItems/Localization/LocalizationBlock.h"
#include "external/commonItems/Localization/LocalizationDatabase.h"
#include "external/fmt/include/fmt/format.h"



namespace
{

commonItems::LocalizationDatabase ConvertCountryLocalizations(
    const commonItems::LocalizationDatabase& vic3_localizations,
    const std::map<std::string, std::string>& country_mappings)
{
   commonItems::LocalizationDatabase country_localizations("english",
       {"braz_por", "french", "german", "japanese", "polish", "russian", "spanish"});

   for (const auto& [vic3_country, hoi4_country]: country_mappings)
   {
      const auto& country_localization_block = vic3_localizations.GetLocalizationBlock(vic3_country);
      if (!country_localization_block)
      {
         continue;
      }

      country_localizations.AddOrModifyLocalizationBlock(hoi4_country, *country_localization_block);
      country_localizations.AddOrModifyLocalizationBlock(fmt::format("{}_DEF", hoi4_country),
          *country_localization_block);

      const auto& adjective_localization_block =
          vic3_localizations.GetLocalizationBlock(fmt::format("{}_ADJ", vic3_country));
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

   for (const auto& [region_name, state_region]: vic3_state_regions)
   {
      for (const auto& [vic3_province, type]: state_region.GetSignificantProvinces())
      {
         const auto& hoi4_provinces = province_mapper.GetVic3ToHoi4ProvinceMapping(vic3_province);
         if (hoi4_provinces.empty())
         {
            ++skipped;
            continue;
         }

         const auto& vic3_localization =
             vic3_localizations.GetLocalizationBlock(fmt::format("HUB_NAME_{}_{}", region_name, type));
         if (!vic3_localization.has_value())
         {
            ++skipped;
            continue;
         }

         victory_point_localizations.AddOrModifyLocalizationBlock(
             fmt::format("VICTORY_POINTS_{}", *hoi4_provinces.begin()),
             *vic3_localization);
      }
   }

   Log(LogLevel::Info) << skipped << " VP localizations were skipped.";

   return victory_point_localizations;
}

}  // namespace



hoi4::Localizations hoi4::ConvertLocalizations(const commonItems::LocalizationDatabase& vic3_localizations,
    const std::map<std::string, std::string>& country_mappings,
    const std::map<std::string, std::string>& hoi4_state_names_to_vic3_state_names,
    const std::map<std::string, vic3::StateRegion>& vic3_state_regions,
    const mappers::ProvinceMapper& province_mapper)
{
   Log(LogLevel::Info) << "\tConverting localizations";
   commonItems::LocalizationDatabase country_localizations =
       ConvertCountryLocalizations(vic3_localizations, country_mappings);
   commonItems::LocalizationDatabase state_localizations =
       ConvertStateLocalizations(vic3_localizations, hoi4_state_names_to_vic3_state_names);
   commonItems::LocalizationDatabase victory_point_localizations =
       ConvertVictoryPointLocalizations(vic3_localizations, vic3_state_regions, province_mapper);

   return {country_localizations, state_localizations, victory_point_localizations};
}