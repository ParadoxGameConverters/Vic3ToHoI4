#include "vic3_to_hoi4_converter.h"

#include "external/commonItems/Log.h"
#include "external/commonItems/ModLoader/ModFilesystem.h"
#include "src/hoi4_world/world/hoi4_world.h"
#include "src/hoi4_world/world/hoi4_world_converter.h"
#include "src/out_hoi4/out_mod.h"
#include "src/out_hoi4/world/out_world.h"
#include "src/vic3_world/world/vic3_world.h"
#include "src/vic3_world/world/vic3_world_importer.h"



void ConvertVic3ToHoi4(const configuration::Configuration& configuration, const GameVersion& game_version)
{
   const auto source_world = vic3::ImportWorld(configuration);

   const commonItems::ModFilesystem hoi4_mod_filesystem(configuration.hoi4_directory, {});

   const auto world_mapper = mappers::WorldMapperBuilder::DefaultMapper(hoi4_mod_filesystem, source_world).Build();
   world_mapper.province_mapper.CheckAllVic3ProvincesMapped(source_world.GetProvinceDefinitions().GetProvinceDefinitions());
   const hoi4::World destination_world =
       hoi4::ConvertWorld(hoi4_mod_filesystem, source_world, world_mapper, configuration.debug);

   out::ClearOutputFolder(configuration.output_name);
   out::OutputMod(configuration.output_name, game_version);
   out::OutputWorld(configuration.output_name, destination_world);
   Log(LogLevel::Progress) << "100%";
   Log(LogLevel::Notice) << "* Conversion complete *";
}