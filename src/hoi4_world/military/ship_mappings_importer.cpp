#include "src/hoi4_world/military/ship_mappings_importer.hpp"



namespace hoi4
{

// todo - get this from a configurable
std::map<std::string, hoi4::Ship> ImportShipMappings([[maybe_unused]] std::string_view input_file)
{
   return {
       {
           "ship_type_aircraft_carrier",
           hoi4::Ship{
               .name = "Carrier",
               .definition = "battleship",
               .equipment = "ship_hull_carrier_1",
               .legacy_equipment = "carrier_2",
               .version = "1936 Carrier",
           },
       },
       {
           "ship_type_super_dreadnought",
           hoi4::Ship{
               .name = "Battleship",
               .definition = "battleship",
               .equipment = "ship_hull_heavy_2",
               .legacy_equipment = "battleship_2",
               .version = "1936 Battleship",
           },
       },
       {
           "ship_type_dreadnought",
           hoi4::Ship{
               .name = "Battleship",
               .definition = "battleship",
               .equipment = "ship_hull_heavy_1",
               .legacy_equipment = "battleship_1",
               .version = "Early Battleship",
           },
       },
       //{
       //    "",
       //    hoi4::Ship{
       //        .name = "Heavy Cruiser",
       //        .definition = "heavy_cruiser",
       //        .equipment = "ship_hull_cruiser_2",
       //        .legacy_equipment = "heavy_cruiser_2",
       //        .version = "1936 Heavy Cruiser",
       //    },
       //},
       {
           "ship_type_seaplane_tender",
           hoi4::Ship{
               .name = "Heavy Cruiser",
               .definition = "heavy_cruiser",
               .equipment = "ship_hull_cruiser_1",
               .legacy_equipment = "heavy_cruiser_1",
               .version = "Early Heavy Cruiser",
           },
       },
       {
           "ship_type_light_cruiser",
           hoi4::Ship{
               .name = "Light Cruiser",
               .definition = "light_cruiser",
               .equipment = "ship_hull_cruiser_2",
               .legacy_equipment = "light_cruiser_2",
               .version = "1936 Light Cruiser",
           },
       },
       {
           "ship_type_armored_cruiser",
           hoi4::Ship{
               .name = "Light Cruiser",
               .definition = "light_cruiser",
               .equipment = "ship_hull_cruiser_1",
               .legacy_equipment = "light_cruiser_1",
               .version = "Early Light Cruiser",
           },
       },
       {
           "ship_type_destroyer",
           hoi4::Ship{
               .name = "Destroyer",
               .definition = "destroyer",
               .equipment = "ship_hull_light_2",
               .legacy_equipment = "destroyer_2",
               .version = "1936 Destroyer",
           },
       },
       {
           "ship_type_torpedo_boat_destroyer",
           hoi4::Ship{
               .name = "Destroyer",
               .definition = "destroyer",
               .equipment = "ship_hull_light_1",
               .legacy_equipment = "destroyer_1",
               .version = "Early Destroyer",
           },
       },
       //{
       //    "",
       //    hoi4::Ship{
       //        .name = "Submarine",
       //        .definition = "submarine",
       //        .equipment = "ship_hull_submarine_2",
       //        .legacy_equipment = "submarine_2",
       //        .version = "1936 Submarine",
       //    },
       //},
       {
           "ship_type_submarine",
           hoi4::Ship{
               .name = "Submarine",
               .definition = "submarine",
               .equipment = "ship_hull_submarine_1",
               .legacy_equipment = "submarine_1",
               .version = "Early Submarine",
           },
       },
   };
}

}  // namespace hoi4