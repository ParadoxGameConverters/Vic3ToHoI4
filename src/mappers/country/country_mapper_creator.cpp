#include "src/mappers/country/country_mapper_creator.h"

#include <queue>
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

   std::vector<const vic3::Country*> deferred_map_countries = {};
   // civil war countries are very last.
   std::vector<const vic3::Country*> civilwar_map_countries = {};

   std::set<std::string> used_hoi4_tags;
   for (const vic3::Country& country: countries | std::views::values)
   {
      const auto& vic3_tag = country.GetTag();
      const auto& mappingRule = country_mapping_rules.find(vic3_tag);
      if (!country.IsCivilWarCountry() && mappingRule != country_mapping_rules.end() &&
          used_hoi4_tags.emplace(mappingRule->second).second)
      {
         country_mappings.emplace(country.GetNumber(), mappingRule->second);
      }
      else if (!country.IsCivilWarCountry())
      {
         deferred_map_countries.emplace_back(&country);
      }
      else
      {
         civilwar_map_countries.emplace_back(&country);
      }
   }

   for (const auto& country: deferred_map_countries)
   {
      const auto& vic3_tag = country->GetTag();
      // first, attempt to map the default rule again. This will trigger for civil war countries
      //  that have a different tag from their parent country.
      const auto& mappingRule = country_mapping_rules.find(vic3_tag);
      if (mappingRule != country_mapping_rules.end() && used_hoi4_tags.emplace(mappingRule->second).second)
      {
         country_mappings.emplace(country->GetNumber(), vic3_tag);
      }
      // if that doesn't work, try to use vic3 country tag next
      else if (vic3_tag.length() == 3 && used_hoi4_tags.emplace(vic3_tag).second)
      {
         country_mappings.emplace(country->GetNumber(), vic3_tag);
      }
      else
      {
         std::string possible_hoi4_tag = fmt::format("{}{:0>2}", tag_prefix, tag_suffix);
         while (used_hoi4_tags.contains(possible_hoi4_tag))
         {
            ++tag_suffix;
            if (tag_suffix > 99)
            {
               tag_suffix = 0;
               --tag_prefix;
            }
            possible_hoi4_tag = fmt::format("{}{:0>2}", tag_prefix, tag_suffix);
         }
         country_mappings.emplace(country->GetNumber(), possible_hoi4_tag);
         used_hoi4_tags.emplace(possible_hoi4_tag);
      }
   }

   for (const auto& country: civilwar_map_countries)
   {
      const auto& vic3_tag = country->GetTag();
      // first, attempt to map the default rule again. This will trigger for civil war countries
      //  that have a different tag from their parent country.
      const auto& mappingRule = country_mapping_rules.find(vic3_tag);
      if (mappingRule != country_mapping_rules.end() && used_hoi4_tags.emplace(mappingRule->second).second)
      {
         country_mappings.emplace(country->GetNumber(), vic3_tag);
      }
      // if that doesn't work, try to use vic3 country tag next
      else if (vic3_tag.length() == 3 && used_hoi4_tags.emplace(vic3_tag).second)
      {
         country_mappings.emplace(country->GetNumber(), vic3_tag);
      }
      else
      {
         std::string possible_hoi4_tag = fmt::format("{}{:0>2}", tag_prefix, tag_suffix);
         while (used_hoi4_tags.contains(possible_hoi4_tag))
         {
            ++tag_suffix;
            if (tag_suffix > 99)
            {
               tag_suffix = 0;
               --tag_prefix;
            }
            possible_hoi4_tag = fmt::format("{}{:0>2}", tag_prefix, tag_suffix);
         }
         country_mappings.emplace(country->GetNumber(), possible_hoi4_tag);
         used_hoi4_tags.emplace(possible_hoi4_tag);
      }
   }

   return CountryMapper(country_mappings);
}