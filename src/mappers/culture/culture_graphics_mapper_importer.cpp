#include "src/mappers/culture/culture_graphics_mapper_importer.h"

#include "external/commonItems/CommonRegexes.h"
#include "external/commonItems/ParserHelpers.h"

mappers::CultureGraphicsMapper mappers::ImportCultureGraphicsMapper(std::string_view mapping_file)
{
   std::vector<CultureGraphicsMapping> mappings;

   IdeologyPortraitPaths leader_group;
   IdeologyPortraitPaths advisor_group;
   std::vector<std::string> ideology_leader_group;
   std::vector<std::string> ideology_advisor_group;
   std::vector<std::string> male_group;
   std::vector<std::string> female_group;
   CultureGraphicsMapping mapping;


   commonItems::parser gender_parser;
   gender_parser.registerKeyword("male", [&male_group](std::istream& input_stream) {
      male_group = commonItems::getStrings(input_stream);
   });
   gender_parser.registerKeyword("female", [&female_group](std::istream& input_stream) {
      female_group = commonItems::getStrings(input_stream);
   });
   gender_parser.IgnoreUnregisteredItems();


   commonItems::parser ideology_parser;
   ideology_parser.registerKeyword("leader", [&ideology_leader_group](std::istream& input_stream) {
      ideology_leader_group = commonItems::getStrings(input_stream);
   });
   ideology_parser.registerKeyword("minister", [&ideology_advisor_group](std::istream& input_stream) {
      ideology_advisor_group = commonItems::getStrings(input_stream);
   });
   ideology_parser.IgnoreUnregisteredItems();


   commonItems::parser political_parser;
   political_parser.registerKeyword("communism",
       [&ideology_leader_group, &leader_group, &ideology_advisor_group, &advisor_group, &ideology_parser](
           std::istream& input_stream) {
          ideology_leader_group.clear();
          ideology_advisor_group.clear();
          ideology_parser.parseStream(input_stream);
          leader_group.communism = std::move(ideology_leader_group);
          advisor_group.communism = std::move(ideology_advisor_group);
       });
   political_parser.registerKeyword("democratic",
       [&ideology_leader_group, &leader_group, &ideology_advisor_group, &advisor_group, &ideology_parser](
           std::istream& input_stream) {
          ideology_leader_group.clear();
          ideology_advisor_group.clear();
          ideology_parser.parseStream(input_stream);
          leader_group.democratic = std::move(ideology_leader_group);
          advisor_group.democratic = std::move(ideology_advisor_group);
       });
   political_parser.registerKeyword("fascism",
       [&ideology_leader_group, &leader_group, &ideology_advisor_group, &advisor_group, &ideology_parser](
           std::istream& input_stream) {
          ideology_leader_group.clear();
          ideology_advisor_group.clear();
          ideology_parser.parseStream(input_stream);
          leader_group.fascism = std::move(ideology_leader_group);
          advisor_group.fascism = std::move(ideology_advisor_group);
       });
   political_parser.registerKeyword("neutrality",
       [&ideology_leader_group, &leader_group, &ideology_advisor_group, &advisor_group, &ideology_parser](
           std::istream& input_stream) {
          ideology_leader_group.clear();
          ideology_advisor_group.clear();
          ideology_parser.parseStream(input_stream);
          leader_group.neutrality = std::move(ideology_leader_group);
          advisor_group.neutrality = std::move(ideology_advisor_group);
       });
   political_parser.registerKeyword("anarchist", [&mapping](std::istream& input_stream) {
      mapping.graphics_block.portrait_paths.council = commonItems::getStrings(input_stream);
   });
   political_parser.registerKeyword("monarchist",
       [&mapping, &male_group, &female_group, &gender_parser](std::istream& input_stream) {
          male_group.clear();
          female_group.clear();
          gender_parser.parseStream(input_stream);
          mapping.graphics_block.portrait_paths.male_monarch = std::move(male_group);
          mapping.graphics_block.portrait_paths.female_monarch = std::move(female_group);
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
      mapping.graphics_block.portrait_paths.army = commonItems::getStrings(input_stream);
   });
   mapping_parser.registerKeyword("navy_portraits", [&mapping](std::istream& input_stream) {
      mapping.graphics_block.portrait_paths.navy = commonItems::getStrings(input_stream);
   });
   mapping_parser.registerKeyword("female_portraits", [&mapping](std::istream& input_stream) {
      mapping.graphics_block.portrait_paths.female_leader = commonItems::getStrings(input_stream);
   });
   mapping_parser.registerKeyword("operative_portraits",
       [&mapping, &male_group, &female_group, &gender_parser](std::istream& input_stream) {
          male_group.clear();
          female_group.clear();
          gender_parser.parseStream(input_stream);
          mapping.graphics_block.portrait_paths.male_operative = std::move(male_group);
          mapping.graphics_block.portrait_paths.female_operative = std::move(female_group);
       });
   mapping_parser.registerKeyword("political_portraits",
       [&mapping, &leader_group, &advisor_group, &political_parser](std::istream& input_stream) {
          political_parser.parseStream(input_stream);
          mapping.graphics_block.portrait_paths.leader = std::move(leader_group);
          mapping.graphics_block.portrait_paths.advisor = std::move(advisor_group);
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
