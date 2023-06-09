#include "src/mappers/culture/culture_graphics_mapper_importer.h"

#include "external/commonItems/CommonRegexes.h"
#include "external/commonItems/ParserHelpers.h"

mappers::CultureGraphicsMapper mappers::ImportCultureGraphicsMapper(std::string_view mapping_file)
{
   std::vector<CultureGraphicsMapping> mappings;

   IdeologyPortraitPaths ideology_group;
   CultureGraphicsMapping mapping;

   commonItems::parser ideology_path_parser;
   ideology_path_parser.registerKeyword("communism", [&ideology_group](std::istream& input_stream) {
      ideology_group.communism = commonItems::getStrings(input_stream);
   });
   ideology_path_parser.registerKeyword("democratic", [&ideology_group](std::istream& input_stream) {
      ideology_group.democratic = commonItems::getStrings(input_stream);
   });
   ideology_path_parser.registerKeyword("fascism", [&ideology_group](std::istream& input_stream) {
      ideology_group.fascism = commonItems::getStrings(input_stream);
   });
   ideology_path_parser.registerKeyword("neutrality", [&ideology_group](std::istream& input_stream) {
      ideology_group.neutrality = commonItems::getStrings(input_stream);
   });
   ideology_path_parser.IgnoreUnregisteredItems();


   commonItems::parser mapping_parser;
   mapping_parser.registerKeyword("cultures", [&mapping](std::istream& input_stream) {
      std::vector<std::string> cultures = commonItems::getStrings(input_stream);
      std::ranges::copy(cultures, std::inserter(mapping.cultures, mapping.cultures.end()));
   });
   mapping_parser.registerKeyword("traits", [&mapping](std::istream& input_stream) {
      std::vector<std::string> traits = commonItems::getStrings(input_stream);
      std::ranges::copy(traits, std::inserter(mapping.traits, mapping.traits.end()));
   });
   mapping_parser.registerKeyword("cultures", [&mapping](std::istream& input_stream) {
      std::vector<std::string> ethnicities = commonItems::getStrings(input_stream);
      std::ranges::copy(ethnicities, std::inserter(mapping.ethnicities, mapping.ethnicities.end()));
   });
   mapping_parser.registerKeyword("army_portraits", [&mapping](std::istream& input_stream) {
      mapping.graphics_block.portrait_paths.army = commonItems::getStrings(input_stream);
   });
   mapping_parser.registerKeyword("navy_portraits", [&mapping](std::istream& input_stream) {
      mapping.graphics_block.portrait_paths.navy = commonItems::getStrings(input_stream);
   });
   mapping_parser.registerKeyword("female_portraits", [&mapping](std::istream& input_stream) {
      mapping.graphics_block.portrait_paths.female_leader = commonItems::getStrings(input_stream);
   });
   mapping_parser.registerKeyword("male_operative_portraits", [&mapping](std::istream& input_stream) {
      mapping.graphics_block.portrait_paths.male_operative = commonItems::getStrings(input_stream);
   });
   mapping_parser.registerKeyword("female_operative_portraits", [&mapping](std::istream& input_stream) {
      mapping.graphics_block.portrait_paths.female_operative = commonItems::getStrings(input_stream);
   });
   mapping_parser.registerKeyword("male_monarch", [&mapping](std::istream& input_stream) {
      mapping.graphics_block.portrait_paths.male_monarch = commonItems::getStrings(input_stream);
   });
   mapping_parser.registerKeyword("female_monarch", [&mapping](std::istream& input_stream) {
      mapping.graphics_block.portrait_paths.female_monarch = commonItems::getStrings(input_stream);
   });
   mapping_parser.registerKeyword("leader_portraits",
       [&mapping, &ideology_group, &ideology_path_parser](std::istream& input_stream) {
          ideology_group = IdeologyPortraitPaths();
          ideology_path_parser.parseStream(input_stream);
          mapping.graphics_block.portrait_paths.leader = ideology_group;
       });
   mapping_parser.registerKeyword("ideology_minister_portraits",
       [&mapping, &ideology_group, &ideology_path_parser](std::istream& input_stream) {
          ideology_group = IdeologyPortraitPaths();
          ideology_path_parser.parseStream(input_stream);
          mapping.graphics_block.portrait_paths.ideology_minister = ideology_group;
       });
   mapping_parser.registerKeyword("graphical_culture", [&mapping](std::istream& input_stream) {
      mapping.graphics_block.graphical_culture = commonItems::getString(input_stream);
   });
   mapping_parser.registerKeyword("graphical_culture_2d", [&mapping](std::istream& input_stream) {
      mapping.graphics_block.graphical_culture_2d = commonItems::getString(input_stream);
   });
   mapping_parser.IgnoreUnregisteredItems();

   commonItems::parser file_parser;
   file_parser.registerRegex(commonItems::catchallRegex,
       [&mappings, &mapping, &mapping_parser](const std::string& name, std::istream& input_stream) {
          mapping = CultureGraphicsMapping();
          mapping_parser.parseStream(input_stream);
          mappings.push_back(mapping);
       });
   file_parser.parseFile(mapping_file);

   return CultureGraphicsMapper(mappings);
}
