#include "out_mod.h"

#include <fstream>
#include <string>

#include "external/commonItems/Log.h"
#include "external/commonItems/OSCompatibilityLayer.h"
#include "external/fmt/include/fmt/format.h"
#include "external/fmt/include/fmt/ostream.h"



namespace out
{

namespace
{

void CreateOutputFolder(const std::string_view output_name)
{
   Log(LogLevel::Info) << "\tCopying blank mod";
   if (!commonItems::TryCreateFolder("output"))
   {
      throw std::runtime_error("Could not create output folder");
   }
   if (!commonItems::CopyFolder("blank_mod", fmt::format("output/{}", output_name)))
   {
      throw std::runtime_error("Could not copy blank_mod");
   }

   if (!commonItems::TryCreateFolder(fmt::format("output/{}/common", output_name)))
   {
      throw std::runtime_error(fmt::format("Could not create output/{}/common", output_name));
   }
   if (!commonItems::TryCreateFolder(fmt::format("output/{}/common/countries", output_name)))
   {
      throw std::runtime_error(fmt::format("Could not create output/{}/common/countries", output_name));
   }
   if (!commonItems::TryCreateFolder(fmt::format("output/{}/common/country_tags", output_name)))
   {
      throw std::runtime_error(fmt::format("Could not create output/{}/common/country_tags", output_name));
   }
   if (!commonItems::TryCreateFolder(fmt::format("output/{}/history", output_name)))
   {
      throw std::runtime_error(fmt::format("Could not create output/{}/history", output_name));
   }
   if (!commonItems::TryCreateFolder(fmt::format("output/{}/history/countries", output_name)))
   {
      throw std::runtime_error(fmt::format("Could not create output/{}/history/countries", output_name));
   }
   if (!commonItems::TryCreateFolder(fmt::format("output/{}/history/states", output_name)))
   {
      throw std::runtime_error(fmt::format("Could not create output/{}/history/states", output_name));
   }
   if (!commonItems::TryCreateFolder(fmt::format("output/{}/history/units", output_name)))
   {
      throw std::runtime_error(fmt::format("Could not create output/{}/history/units", output_name));
   }
   if (!commonItems::TryCreateFolder(fmt::format("output/{}/map", output_name)))
   {
      throw std::runtime_error(fmt::format("Could not create output/{}/map", output_name));
   }
   if (!commonItems::TryCreateFolder(fmt::format("output/{}/map/strategicregions", output_name)))
   {
      throw std::runtime_error(fmt::format("Could not create output/{}/map/strategicregions", output_name));
   }
}


void CreateModFiles(const std::string_view output_name, const GameVersion& game_version)
{
   Log(LogLevel::Info) << "\tCreating .mod files";

   std::ofstream mod_file(std::string("output/").append(output_name).append(".mod"));
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

   std::ofstream descriptor_file(std::string("output/").append(output_name).append("/descriptor.mod"));
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


void ClearOutputFolder(std::string_view output_name)
{
   const std::string output_folder = std::string("output/").append(output_name);
   if (commonItems::DoesFolderExist(output_folder))
   {
      Log(LogLevel::Info) << "Removing pre-existing copy of " << output_name;
   }

   if (!commonItems::DeleteFolder(output_folder))
   {
      throw std::runtime_error("Could not remove pre-existing output folder " + output_folder +
                               ". Please delete folder and try converting again.");
   }
}


void OutputMod(const std::string_view output_name, const GameVersion& game_version)
{
   Log(LogLevel::Progress) << "80%";
   Log(LogLevel::Info) << "Outputting mod";

   CreateOutputFolder(output_name);
   CreateModFiles(output_name, game_version);
   Log(LogLevel::Progress) << "85%";
}

}  // namespace out