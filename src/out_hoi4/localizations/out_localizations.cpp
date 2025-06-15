#include "src/out_hoi4/localizations/out_localizations.h"

#include <external/commonItems/CommonFunctions.h>
#include <external/commonItems/OSCompatibilityLayer.h>
#include <external/fmt/include/fmt/format.h>

#include <array>
#include <fstream>



using std::filesystem::path;



namespace
{

constexpr std::array supported_languages =
    {"braz_por", "english", "french", "german", "japanese", "polish", "russian", "spanish"};


void CreateFolders(const path& output_name)
{
   const path localisations_path = "output" / output_name / "localisation";
   if (!commonItems::DoesFolderExist(localisations_path) && !create_directories(localisations_path))
   {
      throw std::runtime_error(fmt::format("Could not create {}", localisations_path.string()));
   }

   for (const auto& language: supported_languages)
   {
      if (const path language_path = localisations_path / language;
          !commonItems::DoesFolderExist(language_path) && !create_directories(language_path))
      {
         throw std::runtime_error(fmt::format("Could not create {}", language_path.string()));
      }
   }
}


void OutputLocalisations(const path& output_name,
    std::string_view localization_file,
    const commonItems::LocalizationDatabase& localization_database)
{
   for (const auto& language: supported_languages)
   {
      const path localization_path(
          "output" / output_name / fmt::format("localisation/{}/{}{}.yml", language, localization_file, language));
      std::ofstream file(localization_path);
      if (!file.is_open())
      {
         throw std::runtime_error(localization_path.string());
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



void out::OutputLocalizations(const path& output_name, const hoi4::Localizations& localizations)
{
   CreateFolders(output_name);
   OutputLocalisations(output_name, "countries_l_", localizations.GetCountryLocalizations());
   OutputLocalisations(output_name, "state_names_l_", localizations.GetStateLocalizations());
   OutputLocalisations(output_name, "victory_points_l_", localizations.GetVictoryPointLocalizations());
   OutputLocalisations(output_name, "character_names_l_", localizations.GetCharacterLocalizations());
   OutputLocalisations(output_name, "converter_ideas_l_", localizations.GetIdeaLocalizations());
}