#include "src/out_hoi4/localizations/out_localizations.h"

#include <fstream>
#include <ranges>

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

   std::ofstream braz_por_file(fmt::format("output/{}/localisation/braz_por/{}", output_name, localization_file));
   if (!braz_por_file.is_open())
   {
      throw std::runtime_error(
          fmt::format("Could not create output/{}/localisation/braz_por/{}", output_name, localization_file));
   }
   braz_por_file << "l_braz_por:\n";

   std::ofstream english_file(fmt::format("output/{}/localisation/english/{}", output_name, localization_file));
   if (!english_file.is_open())
   {
      throw std::runtime_error(
          fmt::format("Could not create output/{}/localisation/english/{}", output_name, localization_file));
   }
   english_file << "l_english:\n";

   for (const auto& localization_block: localization_database | std::views::values)
   {
      const auto& key = localization_block.GetKey();
      english_file << fmt::format(" {}:0 \"{}\"", key, localization_block.GetLocalization("english"));
      braz_por_file << fmt::format(" {}:0 \"{}\"", key, localization_block.GetLocalization("braz_por"));
   }

   english_file.close();
   braz_por_file.close();
}

}  // namespace



void out::OutputLocalizations(std::string_view output_name, const hoi4::Localizations& localizations)
{
   OutputLocalisations(output_name, "test_localisations.yml", localizations.GetTestLocalizations());
}