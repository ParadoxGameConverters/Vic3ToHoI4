#include "out_mod.h"

#include <fstream>
#include <string>

#include "external/commonItems/Log.h"
#include "external/commonItems/OSCompatibilityLayer.h"



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
   if (!commonItems::CopyFolder("blank_mod", std::string("output/").append(output_name)))
   {
      throw std::runtime_error("Could not copy blank_mod");
   }
}


void CreateModFiles(const std::string_view output_name, const GameVersion& game_version)
{
   Log(LogLevel::Info) << "\tCreating .mod files";

   std::ofstream modFile(std::string("output/").append(output_name).append(".mod"));
   if (!modFile.is_open())
   {
      throw std::runtime_error("Could not create .mod file");
   }
   modFile << "name = \"Converted - " << output_name << "\"\n";
   modFile << "path = \"mod/" << output_name << "/\"\n";
   modFile << "user_dir = \"" << output_name << "_user_dir\"\n";
   modFile << "replace_path=\"common/ideologies\"\n";
   modFile << "replace_path=\"history/countries\"\n";
   modFile << "replace_path=\"history/states\"\n";
   modFile << "supported_version=\"" << game_version.toWildCard() << "\"";
   modFile.close();

   std::ofstream descriptorFile(std::string("output/").append(output_name).append("/descriptor.mod"));
   if (!descriptorFile.is_open())
   {
      throw std::runtime_error("Could not create descriptor.mod");
   }
   descriptorFile << "name = \"Converted - " << output_name << "\"\n";
   descriptorFile << "replace_path=\"common/ideologies\"\n";
   descriptorFile << "replace_path=\"history/countries\"\n";
   descriptorFile << "replace_path=\"history/states\"\n";
   descriptorFile << "supported_version=\"" << game_version.toWildCard() << "\"";
   descriptorFile.close();
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


void Output(const std::string_view output_name, const GameVersion& game_version)
{
   Log(LogLevel::Progress) << "80%";
   Log(LogLevel::Info) << "Outputting mod";

   CreateOutputFolder(output_name);
   CreateModFiles(output_name, game_version);
   Log(LogLevel::Progress) << "85%";
}

}  // namespace out