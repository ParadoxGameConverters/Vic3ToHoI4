#ifndef CONFIGURATION_CONFIGURATION_H
#define CONFIGURATION_CONFIGURATION_H



#include <string>


namespace configuration
{

struct Configuration
{
   std::string vic3_directory;
   std::string vic3_steam_mod_path;
   std::string vic3_mod_path;
   std::string hoi4_directory;
   std::string hoi4_mod_path;
   std::string save_game;
   bool debug = false;
   std::string output_name;
};


}  // namespace configuration


#endif  // SRC_CONFIGURATION_CONFIGURATION_H
