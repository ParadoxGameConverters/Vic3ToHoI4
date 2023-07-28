#include "src/mappers/charater/generic_advisor_mapper_importer.h"
﻿
#include "external/commonItems/CommonRegexes.h"
#include "external/commonItems/Parser.h"
#include "external/commonItems/ParserHelpers.h"

mappers::ImportGenericAdvisorMapper(std::string_view mapping_file)
{
   // Outputs
   AdvisorMap advisor_map;

   commonItems::parser slot_parser;
   slot_parser.registerRegex(commonItems::catchallRegex, [&advisor_map](const std::string& slot_type, std::istream& input_stream) {
   for (const hoi4::Trait& trait: commonItems::GetStrings())
   {
      advisor_map.emplace(trait, slot_type);
   }
   });

   slot_parser.parseFile(mapping_file);

   return AdvisorMapper(advisor_map);
}