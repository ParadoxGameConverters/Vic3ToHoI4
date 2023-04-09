#include "src/mappers/country/country_mapper_creator.h"

#include <ranges>

#include "external/commonItems/Parser.h"
#include "external/commonItems/ParserHelpers.h"
#include "external/fmt/include/fmt/format.h"



namespace
{

std::map<std::string, std::string> ImportMappingRules(std::string_view country_mappings_file)
{
   std::map<std::string, std::string> country_mapping_rules;  // vic3 tag -> hoi4 tag

   std::string vic3_tag;
   std::string hoi4_tag;
   commonItems::parser mapping_rule_parser;
   mapping_rule_parser.registerKeyword("vic", [&vic3_tag](std::istream& input_stream) {
      vic3_tag = commonItems::getString(input_stream);
   });
   mapping_rule_parser.registerKeyword("hoi", [&hoi4_tag](std::istream& input_stream) {
      hoi4_tag = commonItems::getString(input_stream);
   });

   commonItems::parser country_mappings_parser;
   country_mappings_parser.registerKeyword("link",
       [&country_mapping_rules, &vic3_tag, &hoi4_tag, &mapping_rule_parser](std::istream& input_stream) {
          vic3_tag.clear();
          hoi4_tag.clear();
          mapping_rule_parser.parseStream(input_stream);
          country_mapping_rules.emplace(vic3_tag, hoi4_tag);
       });

   country_mappings_parser.parseFile(country_mappings_file);

   return country_mapping_rules;
}

}  // namespace


mappers::CountryMapper mappers::CreateCountryMappings(std::string_view country_mappings_file,
    const std::map<int, vic3::Country>& countries)
{
   std::map<int, std::string> country_mappings;

   // vic3 tag -> hoi4 tag
   std::map<std::string, std::string> country_mapping_rules = ImportMappingRules(country_mappings_file);
   char tag_prefix = 'Z';
   int tag_suffix = 0;

   for (const vic3::Country& country: countries | std::views::values)
   {
      const auto& tag = country.GetTag();
      if (const auto& rule = country_mapping_rules.find(tag); rule != country_mapping_rules.end())
      {
         country_mappings.emplace(country.GetNumber(), rule->second);
      }
      else
      {
         country_mappings.emplace(country.GetNumber(), fmt::format("{}{:0>2}", tag_prefix, tag_suffix));

         ++tag_suffix;
         if (tag_suffix > 99)
         {
            tag_suffix = 0;
            --tag_prefix;
         }
      }
   }

   return CountryMapper(country_mappings);
}