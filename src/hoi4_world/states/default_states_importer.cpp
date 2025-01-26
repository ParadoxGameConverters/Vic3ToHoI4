#include "src/hoi4_world/states/default_states_importer.h"

#include <external/commonItems/CommonFunctions.h>
#include <external/commonItems/Parser.h>
#include <external/commonItems/ParserHelpers.h>

#include <istream>



std::map<int, hoi4::DefaultState> hoi4::ImportDefaultStates(const commonItems::ModFilesystem& mod_filesystem)
{
   std::map<int, hoi4::DefaultState> default_states;

   DefaultStateOptions options;

   commonItems::parser state_buildings_parser;
   state_buildings_parser.registerKeyword("industrial_complex", [&options](std::istream& input_stream) {
      options.civilian_factories = commonItems::getInt(input_stream);
   });
   state_buildings_parser.registerKeyword("arms_factory", [&options](std::istream& input_stream) {
      options.military_factories = commonItems::getInt(input_stream);
   });
   state_buildings_parser.registerKeyword("dockyard", [&options](std::istream& input_stream) {
      options.dockyards = commonItems::getInt(input_stream);
   });
   state_buildings_parser.IgnoreUnregisteredItems();

   commonItems::parser state_history_parser;
   state_history_parser.registerKeyword("owner", [&options](std::istream& input_stream) {
      options.owner_tag = commonItems::getString(input_stream);
   });
   state_history_parser.registerKeyword("buildings", [&state_buildings_parser](std::istream& input_stream) {
      state_buildings_parser.parseStream(input_stream);
   });
   state_history_parser.IgnoreUnregisteredItems();

   commonItems::parser default_state_parser;
   default_state_parser.registerKeyword("impassable", [&options](std::istream& input_stream) {
      options.impassable = true;
      commonItems::ignoreItem("", input_stream);
   });
   default_state_parser.registerKeyword("manpower", [&options](std::istream& input_stream) {
      options.manpower = commonItems::getInt(input_stream);
   });
   default_state_parser.registerKeyword("provinces", [&options](std::istream& input_stream) {
      for (int province: commonItems::getInts(input_stream))
      {
         options.provinces.insert(province);
      }
   });
   default_state_parser.registerKeyword("history", [&state_history_parser](std::istream& input_stream) {
      state_history_parser.parseStream(input_stream);
   });
   default_state_parser.registerKeyword("resources", [&options](std::istream& the_stream) {
      for (const auto& [resource, amount]: commonItems::assignments(the_stream).getAssignments())
      {
         options.resources[resource] += std::stod(amount);
      }
   });
   default_state_parser.IgnoreUnregisteredItems();

   commonItems::parser default_state_file_parser;
   default_state_file_parser.registerKeyword("state", [&default_state_parser](std::istream& input_stream) {
      default_state_parser.parseStream(input_stream);
   });
   default_state_file_parser.IgnoreAndLogUnregisteredItems();

   for (const auto& state_file: mod_filesystem.GetAllFilesInFolder("history/states"))
   {
      std::string trimmed_path = state_file.filename().string();
      int num = std::stoi(trimmed_path.substr(0, trimmed_path.find_first_of('-')));

      options.impassable = false;
      options.owner_tag.clear();
      options.provinces.clear();
      options.manpower = 0;
      options.civilian_factories = 0;
      options.military_factories = 0;
      options.dockyards = 0;
      options.resources.clear();

      default_state_file_parser.parseFile(state_file);
      default_states.emplace(num, DefaultState(options));
   }

   return default_states;
}