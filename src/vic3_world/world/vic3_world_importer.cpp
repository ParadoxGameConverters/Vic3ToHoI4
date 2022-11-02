#include "src/vic3_world/world/vic3_world_importer.h"

#include <filesystem>
#include <fstream>
#include <sstream>

#include "external/commonItems/CommonRegexes.h"
#include "external/commonItems/Log.h"
#include "external/commonItems/Parser.h"
#include "external/commonItems/ParserHelpers.h"
#include "external/fmt/include/fmt/format.h"
#include "external/zip/src/zip.h"
#include "src/vic3_world/countries/countries_importer.h"
#include "src/vic3_world/countries/country.h"
#include "src/vic3_world/states/state.h"
#include "src/vic3_world/states/states_importer.h"


namespace
{

constexpr int save_identifier_size = 24;


int GetFileLength(std::istream& file)
{
   file.seekg(0, std::ios_base::end);
   const int length = static_cast<int>(file.tellg());
   file.seekg(save_identifier_size, std::ios_base::beg);

   return length;
}


std::string DecompressSave(std::istream& save)
{
   const auto length = GetFileLength(save);

   char* zipped_buffer = new char[static_cast<uint64_t>(length)];
   save.read(zipped_buffer, length);

   void* unzipped_buffer = nullptr;
   size_t unzipped_size = 0;
   zip_t* zip_file = zip_stream_open(zipped_buffer, static_cast<size_t>(length), 0, 'r');
   if (zip_file == nullptr)
   {
      throw std::runtime_error(
          "The save was not a correctly zipped save. It may have been corrupted, try loading it in Vic3 and saving to "
          "a new file.");
   }
   if (zip_entry_opencasesensitive(zip_file, "gamestate") != 0)
   {
      throw std::runtime_error(
          "The save was not a correctly zipped save. It may have been corrupted, try loading it in Vic3 and saving to "
          "a new file.");
   }
   if (zip_entry_read(zip_file, &unzipped_buffer, &unzipped_size) < 0)
   {
      throw std::runtime_error(
          "The save was not a correctly zipped save. It may have been corrupted, try loading it in Vic3 and saving to "
          "a new file.");
   }
   if (zip_entry_close(zip_file) < 0)
   {
      throw std::runtime_error(
          "The save was not a correctly zipped save. It may have been corrupted, try loading it in Vic3 and saving to "
          "a new file.");
   }
   zip_stream_close(zip_file);

   delete[] zipped_buffer;

   Log(LogLevel::Info) << fmt::format("\tUnzipped save to {:L} bytes", unzipped_size);
   std::string unzipped_file(static_cast<char*>(unzipped_buffer), unzipped_size);

   free(unzipped_buffer);

   return unzipped_file;
}


std::istringstream ImportSave(std::string_view save_filename)
{
   std::ifstream save_file(std::filesystem::u8path(save_filename), std::ios::binary);
   if (!save_file.is_open())
   {
      throw std::runtime_error("Could not open save! Exiting!");
   }

   std::string raw_save_data;

   save_file.seekg(save_identifier_size, std::ios_base::beg);
   char buffer[3];
   save_file.read(buffer, 3);
   if (buffer[0] == 'P' && buffer[1] == 'K')
   {
      raw_save_data = DecompressSave(save_file);
   }
   else
   {
      save_file.seekg(save_identifier_size, std::ios_base::beg);
      copy(std::istreambuf_iterator<char>(save_file),
          std::istreambuf_iterator<char>(),
          std::back_inserter(raw_save_data));
   }

   save_file.close();

   return std::istringstream{raw_save_data};
}

}  // namespace


vic3::World vic3::ImportWorld(std::string_view save_filename)
{
   Log(LogLevel::Info) << "*** Hello Vic3, loading World. ***";

   Log(LogLevel::Info) << "-> Reading Vic3 save.";
   auto save = ImportSave(save_filename);
   Log(LogLevel::Progress) << "7 %";

   // MeltSave();
   Log(LogLevel::Progress) << "9 %";

   Log(LogLevel::Info) << "-> Processing Vic3 save.";
   std::map<int, State> states;
   StatesImporter states_importer;
   std::map<int, Country> countries;
   CountriesImporter countries_importer;


   commonItems::parser save_parser;
   save_parser.registerKeyword("country_manager", [&countries, &countries_importer](std::istream& input_stream) {
      countries = countries_importer.ImportCountries(input_stream);
   });
   save_parser.registerKeyword("states", [&states, &states_importer](std::istream& input_stream) {
      states = states_importer.ImportStates(input_stream);
   });
   save_parser.registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);

   save_parser.parseStream(save);
   Log(LogLevel::Info) << fmt::format("\t{} countries imported", countries.size());
   Log(LogLevel::Info) << fmt::format("\t{} states imported", states.size());
   Log(LogLevel::Progress) << "15 %";

   return World(countries, states);
}