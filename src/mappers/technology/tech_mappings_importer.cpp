#include "src/mappers/technology/tech_mappings_importer.h"

#include <external/commonItems/Parser.h>

#include "src/mappers/technology/tech_mapping_importer.h"



std::vector<mappers::TechMapping> mappers::ImportTechMappings()
{
   std::vector<mappers::TechMapping> tech_mappings;

   TechMappingImporter mapping_importer;

   commonItems::parser parser;
   parser.registerKeyword("link", [&mapping_importer, &tech_mappings](std::istream& theStream) {
      tech_mappings.push_back(mapping_importer.ImportTechMapping(theStream));
   });

   parser.parseFile(std::filesystem::path("configurables/tech_mappings.txt"));

   return tech_mappings;
}