#include "src/vic3_world/world/vic3_world_importer.h"

#include <filesystem>
#include <fstream>
#include <sstream>

#include "external/commonItems/CommonRegexes.h"
#include "external/commonItems/Log.h"
#include "external/commonItems/Parser.h"
#include "external/commonItems/ParserHelpers.h"
#include "external/fmt/include/fmt/format.h"
#include "external/rakaly/rakaly.h"
#include "src/vic3_world/countries/vic3_countries_importer.h"
#include "src/vic3_world/countries/vic3_country.h"
#include "src/vic3_world/provinces/vic3_province_definitions.h"
#include "src/vic3_world/provinces/vic3_province_definitions_loader.h"
#include "src/vic3_world/states/vic3_state.h"
#include "src/vic3_world/states/vic3_states_importer.h"



namespace
{

std::istringstream MeltSave(std::string_view save_filename, bool debug)
{
   std::ifstream save_file(std::filesystem::u8path(save_filename), std::ios::in | std::ios::binary);
   const auto save_size = static_cast<std::streamsize>(std::filesystem::file_size(save_filename));
   std::string save_string(save_size, '\0');
   save_file.read(save_string.data(), save_size);

   const auto game_state = rakaly::meltVic3(save_string);

   if (debug)
   {
      std::ofstream liquid("liquid_save.txt");
      liquid << game_state;
      liquid.close();
   }

   return std::istringstream{game_state};
}

}  // namespace


vic3::World vic3::ImportWorld(std::string_view save_filename, const commonItems::ModFilesystem& mod_filesystem, bool debug)
{
   Log(LogLevel::Info) << "*** Hello Vic3, loading World. ***";

   Log(LogLevel::Info) << "-> Reading Vic3 install.";
   const auto province_definitions = ProvinceDefinitionsLoader().LoadProvinceDefinitions(mod_filesystem);
   Log(LogLevel::Progress) << "5 %";

   Log(LogLevel::Info) << "-> Reading Vic3 save.";
   auto save = MeltSave(save_filename, debug);
   Log(LogLevel::Progress) << "7 %";

   Log(LogLevel::Info) << "-> Processing Vic3 save.";
   std::map<int, State> states;
   StatesImporter states_importer(debug);
   std::map<int, Country> countries;
   CountriesImporter countries_importer(debug);


   commonItems::parser save_parser;
   save_parser.registerKeyword("country_manager", [&countries, &countries_importer](std::istream& input_stream) {
      countries = countries_importer.ImportCountries(input_stream);
   });
   save_parser.registerKeyword("states", [&states, &states_importer](std::istream& input_stream) {
      states = states_importer.ImportStates(input_stream);
   });
   save_parser.registerRegex("SAV.*", [](const std::string& unused, std::istream& input_stream) {
   });
   if (debug)
   {
      save_parser.registerRegex(commonItems::catchallRegex, commonItems::ignoreAndLogItem);
   }
   else
   {
      save_parser.registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
   }

   save_parser.parseStream(save);
   Log(LogLevel::Info) << fmt::format("\t{} countries imported", countries.size());
   Log(LogLevel::Info) << fmt::format("\t{} states imported", states.size());
   Log(LogLevel::Progress) << "15 %";

   return World(countries, states, province_definitions);
}