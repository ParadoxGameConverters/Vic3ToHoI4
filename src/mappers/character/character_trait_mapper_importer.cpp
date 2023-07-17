#include "src/mappers/character/character_trait_mapper_importer.h"

#include "external/commonItems/CommonRegexes.h"
#include "external/commonItems/Parser.h"
#include "external/commonItems/ParserHelpers.h"

mappers::CharacterTraitMapper mappers::ImportCharacterTraitMapper(std::string_view mapping_file)
{
   // Outputs
   AdmiralTraitMap admiral_trait_map;
   GeneralTraitMap general_trait_map;
   SpyTraitMap spy_trait_map;

   // Capture building blocks
   AdmiralTraitMapping admiral_trait_mapping;
   GeneralTraitMapping general_trait_mapping;


   commonItems::parser admiral_mapping_parser;
   admiral_mapping_parser.registerKeyword("traits", [&admiral_trait_mapping](std::istream& input_stream) {
      std::ranges::copy(commonItems::getStrings(input_stream),
          std::inserter(admiral_trait_mapping.traits, admiral_trait_mapping.traits.begin()));
   });
   admiral_mapping_parser.registerKeyword("attack", [&admiral_trait_mapping](std::istream& input_stream) {
      admiral_trait_mapping.attack = commonItems::getInt(input_stream);
   });
   admiral_mapping_parser.registerKeyword("defense", [&admiral_trait_mapping](std::istream& input_stream) {
      admiral_trait_mapping.defense = commonItems::getInt(input_stream);
   });
   admiral_mapping_parser.registerKeyword("maneuvering", [&admiral_trait_mapping](std::istream& input_stream) {
      admiral_trait_mapping.maneuvering = commonItems::getInt(input_stream);
   });
   admiral_mapping_parser.registerKeyword("coordination", [&admiral_trait_mapping](std::istream& input_stream) {
      admiral_trait_mapping.coordination = commonItems::getInt(input_stream);
   });
   admiral_mapping_parser.IgnoreAndLogUnregisteredItems();

   commonItems::parser admiral_parser;
   admiral_parser.registerRegex(commonItems::catchallRegex,
       [&admiral_trait_map, &admiral_trait_mapping, &admiral_mapping_parser](const std::string& vic3_trait,
           std::istream& input_stream) {
          admiral_trait_mapping = AdmiralTraitMapping{};
          admiral_mapping_parser.parseStream(input_stream);
          admiral_trait_map.emplace(vic3_trait, admiral_trait_mapping);
       });


   commonItems::parser general_mapping_parser;
   general_mapping_parser.registerKeyword("traits", [&general_trait_mapping](std::istream& input_stream) {
      std::ranges::copy(commonItems::getStrings(input_stream),
          std::inserter(general_trait_mapping.traits, general_trait_mapping.traits.begin()));
   });
   general_mapping_parser.registerKeyword("field_marshal_traits", [&general_trait_mapping](std::istream& input_stream) {
      std::ranges::copy(commonItems::getStrings(input_stream),
          std::inserter(general_trait_mapping.traits, general_trait_mapping.traits.begin()));
   });
   general_mapping_parser.registerKeyword("attack", [&general_trait_mapping](std::istream& input_stream) {
      general_trait_mapping.attack = commonItems::getInt(input_stream);
   });
   general_mapping_parser.registerKeyword("defense", [&general_trait_mapping](std::istream& input_stream) {
      general_trait_mapping.defense = commonItems::getInt(input_stream);
   });
   general_mapping_parser.registerKeyword("planning", [&general_trait_mapping](std::istream& input_stream) {
      general_trait_mapping.planning = commonItems::getInt(input_stream);
   });
   general_mapping_parser.registerKeyword("logistics", [&general_trait_mapping](std::istream& input_stream) {
      general_trait_mapping.logistics = commonItems::getInt(input_stream);
   });
   general_mapping_parser.IgnoreAndLogUnregisteredItems();

   commonItems::parser general_parser;
   general_parser.registerRegex(commonItems::catchallRegex,
       [&general_trait_map, &general_trait_mapping, &general_mapping_parser](const std::string& vic3_trait,
           std::istream& input_stream) {
          general_trait_mapping = GeneralTraitMapping{};
          general_mapping_parser.parseStream(input_stream);
          general_trait_map.emplace(vic3_trait, general_trait_mapping);
       });


   commonItems::parser spy_parser;
   spy_parser.registerRegex(commonItems::catchallRegex,
       [&spy_trait_map](const std::string& hoi4_trait, std::istream& input_stream) {
          for (const auto& vic3_trait: commonItems::getStrings(input_stream))
          {
             spy_trait_map.emplace(vic3_trait, hoi4_trait);
          }
       });


   commonItems::parser type_parser;
   type_parser.registerKeyword("navy", [&admiral_parser](std::istream& input_stream) {
      admiral_parser.parseStream(input_stream);
   });
   type_parser.registerKeyword("army", [&general_parser](std::istream& input_stream) {
      general_parser.parseStream(input_stream);
   });
   type_parser.registerKeyword("spy", [&spy_parser](std::istream& input_stream) {
      spy_parser.parseStream(input_stream);
   });

   type_parser.parseFile(mapping_file);

   return CharacterTraitMapper(admiral_trait_map, general_trait_map, spy_trait_map);
}