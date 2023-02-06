#include "src/vic3_world/world/vic3_world_importer.h"

#include <filesystem>
#include <fstream>
#include <ranges>
#include <sstream>

#include "external/commonItems/Color.h"
#include "external/commonItems/CommonRegexes.h"
#include "external/commonItems/Log.h"
#include "external/commonItems/ModLoader/Mod.h"
#include "external/commonItems/ModLoader/ModLoader.h"
#include "external/commonItems/Parser.h"
#include "external/commonItems/ParserHelpers.h"
#include "external/fmt/include/fmt/format.h"
#include "external/rakaly/rakaly.h"
#include "src/vic3_world/countries/country_definitions_importer.h"
#include "src/vic3_world/countries/vic3_countries_importer.h"
#include "src/vic3_world/countries/vic3_country.h"
#include "src/vic3_world/provinces/vic3_province_definitions.h"
#include "src/vic3_world/provinces/vic3_province_definitions_loader.h"
#include "src/vic3_world/states/vic3_state.h"
#include "src/vic3_world/states/vic3_states_importer.h"



namespace
{

std::string ReadSave(std::string_view save_filename)
{
   std::ifstream save_file(std::filesystem::u8path(save_filename), std::ios::in | std::ios::binary);
   const auto save_size = static_cast<std::streamsize>(std::filesystem::file_size(save_filename));
   std::string save_string(save_size, '\0');
   save_file.read(save_string.data(), save_size);

   return save_string;
}


std::vector<std::string> ReadModNames(const rakaly::GameFile& save, const std::string& save_string)
{
   std::string save_meta;
   if (save.is_binary())
   {
      const auto melt = save.meltMeta();
      if (!melt || melt->has_unknown_tokens())
      {
         throw std::runtime_error("Unable to melt ironman save's metadata");
      }

      melt->writeData(save_meta);
   }
   else
   {
      save_meta = save_string;
   }
   std::istringstream meta_stream{save_meta};

   std::vector<std::string> mod_names;

   commonItems::parser meta_parser;
   meta_parser.registerKeyword("mods", [&mod_names](std::istream& input_stream) {
      mod_names = commonItems::stringList(input_stream).getStrings();
   });
   meta_parser.IgnoreUnregisteredItems();
   meta_parser.parseStream(meta_stream);

   return mod_names;
}


std::vector<Mod> GetModsFromSave(const rakaly::GameFile& save, const std::string& save_string)
{
   std::vector<std::string> mod_names = ReadModNames(save, save_string);

   std::vector<Mod> mods;
   for (const auto& mod_name: mod_names)
   {
      Mod mod(mod_name, "");
      mods.push_back(mod);
   }

   return mods;
}


std::istringstream MeltSave(const rakaly::GameFile& save, const std::string& save_string)
{
   std::string melted_save_string;
   if (save.is_binary())
   {
      const auto melt = save.melt();
      if (melt.has_unknown_tokens())
      {
         throw std::runtime_error("Unable to melt ironman save");
      }

      melt.writeData(melted_save_string);
   }
   else
   {
      melted_save_string = save_string;
   }

   return std::istringstream{melted_save_string};
}


void AssignOwnersToStates(const std::map<int, vic3::Country>& countries, std::map<int, vic3::State>& states)
{
   for (auto& [state_number, state]: states)
   {
      const auto& possible_owner_number = state.GetOwnerNumber();
      if (!possible_owner_number.has_value())
      {
         continue;
      }

      if (const auto country_itr = countries.find(*possible_owner_number); country_itr != countries.end())
      {
         state.SetOwnerTag(country_itr->second.GetTag());
      }
      else
      {
         Log(LogLevel::Warning) << fmt::format("State {} had an owner with no definition.", state_number);
      }
   }
}

}  // namespace


vic3::World vic3::ImportWorld(const configuration::Configuration& configuration)
{
   Log(LogLevel::Info) << "*** Hello Vic3, loading World. ***";
   std::string save_string = ReadSave(configuration.save_game);
   const rakaly::GameFile save = rakaly::parseVic3(save_string);

   commonItems::ModLoader mod_loader;
   mod_loader.loadMods(std::vector<std::string>{configuration.vic3_mod_path, configuration.vic3_steam_mod_path},
       GetModsFromSave(save, save_string));

   Log(LogLevel::Info) << "-> Reading Vic3 install.";
   commonItems::ModFilesystem mod_filesystem(configuration.vic3_directory, mod_loader.getMods());
   const auto province_definitions = ProvinceDefinitionsLoader().LoadProvinceDefinitions(mod_filesystem);
   Log(LogLevel::Progress) << "5 %";

   Log(LogLevel::Info) << "-> Reading Vic3 save.";
   Log(LogLevel::Progress) << "7 %";
   std::istringstream save_stream = MeltSave(save, save_string);

   Log(LogLevel::Info) << "-> Processing Vic3 save.";
   std::map<int, State> states;
   StatesImporter states_importer;
   std::map<int, Country> countries;
   const std::map<std::string, commonItems::Color> color_definitions = ImportCountryColorDefinitions(mod_filesystem);
   ;
   CountriesImporter countries_importer(color_definitions);


   commonItems::parser save_parser;
   save_parser.registerKeyword("country_manager", [&countries, &countries_importer](std::istream& input_stream) {
      countries = countries_importer.ImportCountries(input_stream);
   });
   save_parser.registerKeyword("states", [&states, &states_importer](std::istream& input_stream) {
      states = states_importer.ImportStates(input_stream);
   });
   save_parser.registerRegex("SAV.*", [](const std::string& unused, std::istream& input_stream) {
   });
   save_parser.IgnoreUnregisteredItems();

   save_parser.parseStream(save_stream);
   Log(LogLevel::Info) << fmt::format("\t{} countries imported", countries.size());
   Log(LogLevel::Info) << fmt::format("\t{} states imported", states.size());
   Log(LogLevel::Progress) << "15 %";

   AssignOwnersToStates(countries, states);

   return World(countries, states, province_definitions);
}