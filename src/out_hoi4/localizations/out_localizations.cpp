#include "src/out_hoi4/localizations/out_localizations.h"

#include <fstream>

#include "external/commonItems/CommonFunctions.h"
#include "external/fmt/include/fmt/format.h"



namespace
{

void OutputLocalisations(std::string_view output_name,
    std::string_view localization_file,
    const commonItems::LocalizationDatabase& localization_database)
{
   if (!commonItems::TryCreateFolder(fmt::format("output/{}/localisation", output_name)))
   {
      throw std::runtime_error(fmt::format("Could not create output/{}/localisation", output_name));
   }
   if (!commonItems::TryCreateFolder(fmt::format("output/{}/localisation/braz_por", output_name)))
   {
      throw std::runtime_error(fmt::format("Could not create output/{}/localisation/braz_por", output_name));
   }
   if (!commonItems::TryCreateFolder(fmt::format("output/{}/localisation/english", output_name)))
   {
      throw std::runtime_error(fmt::format("Could not create output/{}/localisation/english", output_name));
   }
   if (!commonItems::TryCreateFolder(fmt::format("output/{}/localisation/french", output_name)))
   {
      throw std::runtime_error(fmt::format("Could not create output/{}/localisation/french", output_name));
   }
   if (!commonItems::TryCreateFolder(fmt::format("output/{}/localisation/german", output_name)))
   {
      throw std::runtime_error(fmt::format("Could not create output/{}/localisation/german", output_name));
   }
   if (!commonItems::TryCreateFolder(fmt::format("output/{}/localisation/japanese", output_name)))
   {
      throw std::runtime_error(fmt::format("Could not create output/{}/localisation/japanese", output_name));
   }
   if (!commonItems::TryCreateFolder(fmt::format("output/{}/localisation/polish", output_name)))
   {
      throw std::runtime_error(fmt::format("Could not create output/{}/localisation/polish", output_name));
   }
   if (!commonItems::TryCreateFolder(fmt::format("output/{}/localisation/russian", output_name)))
   {
      throw std::runtime_error(fmt::format("Could not create output/{}/localisation/russian", output_name));
   }
   if (!commonItems::TryCreateFolder(fmt::format("output/{}/localisation/spanish", output_name)))
   {
      throw std::runtime_error(fmt::format("Could not create output/{}/localisation/spanish", output_name));
   }

   std::ofstream braz_por_file(fmt::format("output/{}/localisation/braz_por/{}",
       output_name,
       fmt::format("{}braz_por.yml", localization_file)));
   if (!braz_por_file.is_open())
   {
      throw std::runtime_error(fmt::format("Could not create output/{}/localisation/braz_por/{}",
          output_name,
          fmt::format("{}braz_por.yml", localization_file)));
   }
   braz_por_file << commonItems::utf8BOM << "l_braz_por:\n";
   std::ofstream english_file(
       fmt::format("output/{}/localisation/english/{}", output_name, fmt::format("{}english.yml", localization_file)));
   if (!english_file.is_open())
   {
      throw std::runtime_error(fmt::format("Could not create output/{}/localisation/english/{}",
          output_name,
          fmt::format("{}english.yml", localization_file)));
   }
   english_file << commonItems::utf8BOM << "l_english:\n";
   std::ofstream french_file(
       fmt::format("output/{}/localisation/french/{}", output_name, fmt::format("{}french.yml", localization_file)));
   if (!french_file.is_open())
   {
      throw std::runtime_error(fmt::format("Could not create output/{}/localisation/french/{}",
          output_name,
          fmt::format("{}french.yml", localization_file)));
   }
   french_file << commonItems::utf8BOM << "l_french:\n";
   std::ofstream german_file(
       fmt::format("output/{}/localisation/german/{}", output_name, fmt::format("{}german.yml", localization_file)));
   if (!german_file.is_open())
   {
      throw std::runtime_error(fmt::format("Could not create output/{}/localisation/german/{}",
          output_name,
          fmt::format("{}german.yml", localization_file)));
   }
   german_file << commonItems::utf8BOM << "l_german:\n";
   std::ofstream japanese_file(fmt::format("output/{}/localisation/japanese/{}",
       output_name,
       fmt::format("{}japanese.yml", localization_file)));
   if (!japanese_file.is_open())
   {
      throw std::runtime_error(fmt::format("Could not create output/{}/localisation/japanese/{}",
          output_name,
          fmt::format("{}japanese.yml", localization_file)));
   }
   japanese_file << commonItems::utf8BOM << "l_japanese:\n";
   std::ofstream polish_file(
       fmt::format("output/{}/localisation/polish/{}", output_name, fmt::format("{}polish.yml", localization_file)));
   if (!polish_file.is_open())
   {
      throw std::runtime_error(fmt::format("Could not create output/{}/localisation/polish/{}",
          output_name,
          fmt::format("{}polish.yml", localization_file)));
   }
   polish_file << commonItems::utf8BOM << "l_polish:\n";
   std::ofstream russian_file(
       fmt::format("output/{}/localisation/russian/{}", output_name, fmt::format("{}russian.yml", localization_file)));
   if (!russian_file.is_open())
   {
      throw std::runtime_error(fmt::format("Could not create output/{}/localisation/russian/{}",
          output_name,
          fmt::format("{}russian.yml", localization_file)));
   }
   russian_file << commonItems::utf8BOM << "l_russian:\n";
   std::ofstream spanish_file(
       fmt::format("output/{}/localisation/spanish/{}", output_name, fmt::format("{}spanish.yml", localization_file)));
   if (!spanish_file.is_open())
   {
      throw std::runtime_error(fmt::format("Could not create output/{}/localisation/spanish/{}",
          output_name,
          fmt::format("{}spanish.yml", localization_file)));
   }
   spanish_file << commonItems::utf8BOM << "l_spanish:\n";

   for (const auto& [key, localization_block]: localization_database)
   {
      braz_por_file << fmt::format(" {}:0 \"{}\"\n", key, localization_block.GetLocalization("braz_por"));
      english_file << fmt::format(" {}:0 \"{}\"\n", key, localization_block.GetLocalization("english"));
      french_file << fmt::format(" {}:0 \"{}\"\n", key, localization_block.GetLocalization("french"));
      german_file << fmt::format(" {}:0 \"{}\"\n", key, localization_block.GetLocalization("german"));
      japanese_file << fmt::format(" {}:0 \"{}\"\n", key, localization_block.GetLocalization("japanese"));
      polish_file << fmt::format(" {}:0 \"{}\"\n", key, localization_block.GetLocalization("polish"));
      russian_file << fmt::format(" {}:0 \"{}\"\n", key, localization_block.GetLocalization("russian"));
      spanish_file << fmt::format(" {}:0 \"{}\"\n", key, localization_block.GetLocalization("spanish"));
   }

   braz_por_file.close();
   english_file.close();
   french_file.close();
   german_file.close();
   japanese_file.close();
   polish_file.close();
   russian_file.close();
   spanish_file.close();
}

}  // namespace



void out::OutputLocalizations(std::string_view output_name, const hoi4::Localizations& localizations)
{
   OutputLocalisations(output_name, "countries_l_", localizations.GetCountryLocalizations());
}