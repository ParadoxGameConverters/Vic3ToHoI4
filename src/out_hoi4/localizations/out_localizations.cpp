#include "src/out_hoi4/localizations/out_localizations.h"

#include <fstream>

#include "external/commonItems/CommonFunctions.h"
#include "external/fmt/include/fmt/format.h"



namespace
{

constexpr std::array supported_languages =
    {"braz_por", "english", "french", "german", "japanese", "polish", "russian", "spanish"};


void CreateFolders(std::string_view output_name)
{
   if (!commonItems::TryCreateFolder(fmt::format("output/{}/localisation", output_name)))
   {
      throw std::runtime_error(fmt::format("Could not create output/{}/localisation", output_name));
   }

   for (const auto& language: supported_languages)
   {
      if (!commonItems::TryCreateFolder(fmt::format("output/{}/localisation/{}", output_name, language)))
      {
         throw std::runtime_error(fmt::format("Could not create output/{}/localisation/{}", output_name, language));
      }
   }
}


void OutputLocalisations(std::string_view output_name,
    std::string_view localization_file,
    const commonItems::LocalizationDatabase& localization_database)
{
   for (const auto& language: supported_languages)
   {
      std::ofstream file(
          fmt::format("output/{}/localisation/{}/{}{}.yml", output_name, language, localization_file, language));
      if (!file.is_open())
      {
         throw std::runtime_error(fmt::format("Could not create output/{}/localisation/{}/{}{}.yml",
             output_name,
             language,
             localization_file,
             language));
      }
      file << fmt::format("{}l_{}:\n", commonItems::utf8BOM, language);

      for (const auto& [key, localization_block]: localization_database)
      {
         file << fmt::format(" {}:0 \"{}\"\n", key, localization_block.GetLocalization(language));
      }

      file.close();
   }
}

}  // namespace



void out::OutputLocalizations(std::string_view output_name, const hoi4::Localizations& localizations)
{
   CreateFolders(output_name);
   OutputLocalisations(output_name, "countries_l_", localizations.GetCountryLocalizations());
   OutputLocalisations(output_name, "state_names_l_", localizations.GetStateLocalizations());
   OutputLocalisations(output_name, "victory_points_l_", localizations.GetVictoryPointLocalizations());
}