#include "src/hoi4_world/military/division_templates_importer.h"

#include "external/commonItems/CommonRegexes.h"
#include "external/commonItems/Parser.h"
#include "external/commonItems/ParserHelpers.h"



std::vector<hoi4::DivisionTemplate> hoi4::ImportDivisionTemplates(std::string_view templates_filename)
{
   std::vector<DivisionTemplate> division_templates;

   std::string name;
   UnitCount battalions;
   UnitCount support;

   commonItems::parser template_parser;
   template_parser.registerKeyword("name", [&name](std::istream& input_stream) {
      name = commonItems::remQuotes(commonItems::getString(input_stream));
   });
   template_parser.registerKeyword("regiments", [&battalions](std::istream& input_stream) {
      auto unit_types = commonItems::stringsOfItemNames(input_stream).getStrings();
      for (const auto& ut: unit_types)
      {
         battalions[ut]++;
      }
   });
   template_parser.registerKeyword("support", [&support](std::istream& input_stream) {
      auto unit_types = commonItems::stringsOfItemNames(input_stream).getStrings();
      for (const auto& ut: unit_types)
      {
         support[ut]++;
      }
   });
   template_parser.registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);

   commonItems::parser file_parser;
   file_parser.registerKeyword("division_template",
       [&division_templates, &name, &battalions, &support, &template_parser](std::istream& input_stream) {
          name.clear();
          battalions.clear();
          support.clear();
          template_parser.parseStream(input_stream);
          division_templates.emplace_back(name, battalions, support);
       });
   file_parser.registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);

   file_parser.parseFile(templates_filename);

   return division_templates;
}
