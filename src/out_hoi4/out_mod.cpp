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

void CreateSubfolder(const path& output_name, std::string_view subfolder)
{
   if (const path path = "output" / output_name / subfolder;
       !commonItems::DoesFolderExist(path) && !create_directories(path))
   {
      throw std::runtime_error(fmt::format("Could not create {}", path.string()));
   }
}


void CreateOutputFolder(const path& output_name)
{
   Log(LogLevel::Info) << "\tCopying blank mod";
   if (!commonItems::DoesFolderExist("output"))
   {
      if (!create_directories("output"))
      {
         throw std::runtime_error("Could not create output folder");
      }
   }
   try
   {
      copy("blank_mod", "output" / output_name, copy_options::recursive);
   }
   catch (...)
   {
      throw std::runtime_error("Could not copy blank_mod");
   }

   CreateSubfolder(output_name, "common");
   CreateSubfolder(output_name, "common/countries");
   CreateSubfolder(output_name, "common/country_tags");
   CreateSubfolder(output_name, "history");
   CreateSubfolder(output_name, "history/countries");
   CreateSubfolder(output_name, "history/states");
   CreateSubfolder(output_name, "history/units");
   CreateSubfolder(output_name, "map");
   CreateSubfolder(output_name, "map/strategicregions");
}


void CreateModFiles(std::string_view output_name, const GameVersion& game_version)
{
   Log(LogLevel::Info) << "\tCreating .mod files";

   std::ofstream mod_file(fmt::format("output/{}.mod", output_name));
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

   std::ofstream descriptor_file(fmt::format("output/{}/descriptor.mod", output_name));
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