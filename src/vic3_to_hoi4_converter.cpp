#include "vic3_to_hoi4_converter.h"

#include "external/commonItems/Log.h"
#include "src/hoi4_world/world/world.h"
#include "src/mappers/country/country_mapper.h"
#include "src/out_hoi4/out_mod.h"
#include "src/vic3_world/world/vic3_world.h"
#include "src/vic3_world/world/vic3_world_importer.h"



void ConvertVic3ToHoi4(const configuration::Configuration& configuration, const GameVersion& game_version)
{
   const auto source_world = vic3::ImportWorld(configuration.save_game);

   constexpr mappers::CountryMapper country_mapper;

   const hoi4::World destination_world(source_world, country_mapper);

   out::ClearOutputFolder(configuration.output_name);
   out::Output(configuration.output_name, game_version);
   Log(LogLevel::Progress) << "100%";
   Log(LogLevel::Notice) << "* Conversion complete *";
}