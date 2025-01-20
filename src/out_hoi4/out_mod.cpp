#include "out_mod.h"

#include <external/commonItems/Log.h>
#include <external/commonItems/OSCompatibilityLayer.h>
#include <external/fmt/include/fmt/format.h>
#include <external/fmt/include/fmt/ostream.h>

#include <fstream>
#include <string>



using std::filesystem::copy_options;
using std::filesystem::create_directories;
using std::filesystem::path;



namespace out
{

namespace
{

void CreateOutputFolder(const path& output_name)
{
   Log(LogLevel::Info) << "\tCopying blank mod";
   if (!commonItems::DoesFolderExist(path("output")))
   {
      if (!create_directories("output"))
      {
         throw std::runtime_error("Could not create output folder");
      }
   }
   try
   {
      copy(path("blank_mod"), "output" / output_name, copy_options::recursive);
   }
   catch (...)
   {
      throw std::runtime_error("Could not copy blank_mod");
   }

   if (const path common_path = "output" / output_name / "common";
       !commonItems::DoesFolderExist(common_path) && !create_directories(common_path))
   {
      throw std::runtime_error(fmt::format("Could not create {}", common_path.string()));
   }
   if (const path countries_path = "output" / output_name / "common/countries";
       !commonItems::DoesFolderExist(countries_path) && !create_directories(countries_path))
   {
      throw std::runtime_error(fmt::format("Could not create {}", countries_path.string()));
   }
   if (const path country_tags_path = "output" / output_name / "common/country_tags";
       !commonItems::DoesFolderExist(country_tags_path) && !create_directories(country_tags_path))
   {
      throw std::runtime_error(fmt::format("Could not create {}", country_tags_path.string()));
   }
   if (const path history_path = "output" / output_name / "history";
       !commonItems::DoesFolderExist(history_path) && !create_directories(history_path))
   {
      throw std::runtime_error(fmt::format("Could not create {}", history_path.string()));
   }
   if (const path country_history_path = "output" / output_name / "history/countries";
       !commonItems::DoesFolderExist(country_history_path) && !create_directories(country_history_path))
   {
      throw std::runtime_error(fmt::format("Could not create {}", country_history_path.string()));
   }
   if (const path state_history_path = "output" / output_name / "history/states";
       !commonItems::DoesFolderExist(state_history_path) && !create_directories(state_history_path))
   {
      throw std::runtime_error(fmt::format("Could not create {}", state_history_path.string()));
   }
   if (const path unit_history_path = "output" / output_name / "history/units";
       !commonItems::DoesFolderExist(unit_history_path) && !create_directories(unit_history_path))
   {
      throw std::runtime_error(fmt::format("Could not create {}", unit_history_path.string()));
   }
   if (const path map_path = "output" / output_name / "map";
       !commonItems::DoesFolderExist(map_path) && !create_directories(map_path))
   {
      throw std::runtime_error(fmt::format("Could not create {}", map_path.string()));
   }
   if (const path strategicregions_path = "output" / output_name / "map/strategicregions";
       !commonItems::DoesFolderExist(strategicregions_path) && !create_directories(strategicregions_path))
   {
      throw std::runtime_error(fmt::format("Could not create {}", strategicregions_path.string()));
   }
}


void CreateModFiles(std::string_view output_name, const GameVersion& game_version)
{
   Log(LogLevel::Info) << "\tCreating .mod files";

   std::ofstream mod_file(path("output") / fmt::format("{}.mod", output_name));
   if (!mod_file.is_open())
   {
      throw std::runtime_error("Could not create .mod file");
   }
   fmt::print(mod_file,
       "name = \"Converted - {}\"\n"
       "path = \"mod/{}/\"\n"
       "user_dir = \"{}_user_dir\"\n"
       "replace_path=\"common/countries\"\n"
       "replace_path=\"common/national_focus\"\n"
       "replace_path=\"common/peace_conference/ai_peace\"\n"
       "replace_path=\"common/peace_conference/cost_modifiers\"\n"
       "replace_path=\"events\"\n"
       "replace_path=\"history/countries\"\n"
       "replace_path=\"history/states\"\n"
       "replace_path=\"history/units\"\n"
       "replace_path=\"map/supplyareas\"\n"
       "replace_path=\"map/strategicregions\"\n"
       "supported_version=\"{}\"",
       output_name,
       output_name,
       output_name,
       game_version.toWildCard());
   mod_file.close();

   std::ofstream descriptor_file("output" / path(output_name) / "descriptor.mod");
   if (!descriptor_file.is_open())
   {
      throw std::runtime_error("Could not create descriptor.mod");
   }
   fmt::print(descriptor_file,
       "name = \"Converted - {}\"\n"
       "replace_path=\"common/countries\"\n"
       "replace_path=\"common/national_focus\"\n"
       "replace_path=\"common/peace_conference/ai_peace\"\n"
       "replace_path=\"common/peace_conference/cost_modifiers\"\n"
       "replace_path=\"events\"\n"
       "replace_path=\"history/countries\"\n"
       "replace_path=\"history/states\"\n"
       "replace_path=\"history/units\"\n"
       "replace_path=\"map/supplyareas\"\n"
       "replace_path=\"map/strategicregions\"\n"
       "supported_version=\"{}\"",
       output_name,
       game_version.toWildCard());
   descriptor_file.close();
}

}  // namespace


void ClearOutputFolder(const path& output_name)
{
   const path output_folder = "output" / output_name;
   if (commonItems::DoesFolderExist(output_folder))
   {
      Log(LogLevel::Info) << "Removing pre-existing copy of " << output_name.string();
      if (remove_all(output_folder) == static_cast<std::uintmax_t>(-1))
      {
         throw std::runtime_error("Could not remove pre-existing output folder " + output_folder.string() +
                                  ". Please delete folder and try converting again.");
      }
   }
}


void OutputMod(std::string_view output_name, const GameVersion& game_version)
{
   Log(LogLevel::Progress) << "80%";
   Log(LogLevel::Info) << "Outputting mod";

   CreateOutputFolder(output_name);
   CreateModFiles(output_name, game_version);
   Log(LogLevel::Progress) << "85%";
}

}  // namespace out