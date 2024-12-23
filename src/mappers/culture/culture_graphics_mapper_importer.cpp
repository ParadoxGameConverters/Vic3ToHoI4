#include "src/mappers/culture/culture_graphics_mapper_importer.h"

#include <external/commonItems/CommonRegexes.h>
#include <external/commonItems/Log.h>
#include <external/commonItems/ParserHelpers.h>
#include <external/fmt/include/fmt/format.h>



mappers::CultureGraphicsMapper mappers::ImportCultureGraphicsMapper(std::string_view mapping_file)
{
   std::vector<CultureGraphicsMapping> mappings;


   std::string ideology;
   std::string gender_prefix;
   CultureGraphicsMapping mapping;


   commonItems::parser gender_parser;
   gender_parser.registerKeyword("male",
       [&mapping, &gender_prefix](const std::string& gender, std::istream& input_stream) {
          mapping.graphics_block.portrait_paths[fmt::format("{}_{}", gender_prefix, gender)] =
              commonItems::getStrings(input_stream);
       });
   gender_parser.registerKeyword("female",
       [&mapping, &gender_prefix](const std::string& gender, std::istream& input_stream) {
          mapping.graphics_block.portrait_paths[fmt::format("{}_{}", gender_prefix, gender)] =
              commonItems::getStrings(input_stream);
       });
   gender_parser.IgnoreUnregisteredItems();


   commonItems::parser ideology_parser;
   ideology_parser.registerKeyword("leader", [&mapping, &ideology](std::istream& input_stream) {
      mapping.graphics_block.portrait_paths[fmt::format("leader_{}", ideology)] = commonItems::getStrings(input_stream);
   });
   ideology_parser.registerKeyword("minister", [&mapping, &ideology](std::istream& input_stream) {
      mapping.graphics_block.portrait_paths[fmt::format("advisor_{}", ideology)] =
          commonItems::getStrings(input_stream);
   });
   ideology_parser.IgnoreUnregisteredItems();


   commonItems::parser political_parser;
   political_parser.registerKeyword("communism",
       [&ideology, &ideology_parser](const std::string& detected_ideology, std::istream& input_stream) {
          ideology = detected_ideology;
          ideology_parser.parseStream(input_stream);
       });
   political_parser.registerKeyword("democratic",
       [&ideology, &ideology_parser](const std::string& detected_ideology, std::istream& input_stream) {
          ideology = detected_ideology;
          ideology_parser.parseStream(input_stream);
       });
   political_parser.registerKeyword("fascism",
       [&ideology, &ideology_parser](const std::string& detected_ideology, std::istream& input_stream) {
          ideology = detected_ideology;
          ideology_parser.parseStream(input_stream);
       });
   political_parser.registerKeyword("neutrality",
       [&ideology, &ideology_parser](const std::string& detected_ideology, std::istream& input_stream) {
          ideology = detected_ideology;
          ideology_parser.parseStream(input_stream);
       });
   political_parser.registerKeyword("anarchist", [&mapping](std::istream& input_stream) {
      mapping.graphics_block.portrait_paths["council"] = commonItems::getStrings(input_stream);
   });
   political_parser.registerKeyword("monarchist", [&gender_prefix, &gender_parser](std::istream& input_stream) {
      gender_prefix = "monarch";
      gender_parser.parseStream(input_stream);
   });
   political_parser.IgnoreUnregisteredItems();



   commonItems::parser mapping_parser;
   mapping_parser.registerKeyword("cultures", [&mapping](std::istream& input_stream) {
      std::vector<std::string> cultures = commonItems::getStrings(input_stream);
      std::ranges::copy(cultures, std::inserter(mapping.cultures, mapping.cultures.end()));
   });
   mapping_parser.registerKeyword("traits", [&mapping](std::istream& input_stream) {
      std::vector<std::string> traits = commonItems::getStrings(input_stream);
      std::ranges::copy(traits, std::inserter(mapping.traits, mapping.traits.end()));
   });
   mapping_parser.registerKeyword("ethnicities", [&mapping](std::istream& input_stream) {
      std::vector<std::string> ethnicities = commonItems::getStrings(input_stream);
      std::ranges::copy(ethnicities, std::inserter(mapping.ethnicities, mapping.ethnicities.end()));
   });
   mapping_parser.registerKeyword("army_portraits", [&mapping](std::istream& input_stream) {
      mapping.graphics_block.portrait_paths["army"] = commonItems::getStrings(input_stream);
   });
   mapping_parser.registerKeyword("navy_portraits", [&mapping](std::istream& input_stream) {
      mapping.graphics_block.portrait_paths["navy"] = commonItems::getStrings(input_stream);
   });
   mapping_parser.registerKeyword("scientist_portraits", [&gender_prefix, &gender_parser](std::istream& input_stream) {
      gender_prefix = "scientist";
      gender_parser.parseStream(input_stream);
   });
   mapping_parser.registerKeyword("female_portraits", [&mapping](std::istream& input_stream) {
      mapping.graphics_block.portrait_paths["female_leader"] = commonItems::getStrings(input_stream);
   });
   mapping_parser.registerKeyword("operative_portraits", [&gender_prefix, &gender_parser](std::istream& input_stream) {
      gender_prefix = "operative";
      gender_parser.parseStream(input_stream);
   });
   mapping_parser.registerKeyword("political_portraits", [&political_parser](std::istream& input_stream) {
      political_parser.parseStream(input_stream);
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

   Log(LogLevel::Info) << fmt::format("\tImported {} culture graphics mappings.", mappings.size());
   return CultureGraphicsMapper(mappings);
}
