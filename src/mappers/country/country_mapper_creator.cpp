#include "src/mappers/country/country_mapper_creator.h"

#include <execution>
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

/// Class so that different mapping strategies can share global variables like country_mappings and
/// country_mapping_rules
mappers::CountryMappingCreator::CountryMappingCreator(std::string_view country_mappings_file)
{
   country_mapping_rules_ = ImportMappingRules(country_mappings_file);

   DeferCountryWithCivilWar = [this](const vic3::Country& country) -> bool {
      if (country.IsCivilWarCountry())
      {
         deferred_map_countries_.push_back(&country);
         return true;
      }
      return false;
   };

   AddCountryWithRule = [this](const vic3::Country& country) -> bool {
      const auto& vic3_tag = country.GetTag();
      const auto& mappingRule = country_mapping_rules_.find(vic3_tag);
      if (mappingRule != country_mapping_rules_.end() && used_hoi4_tags_.emplace(mappingRule->second).second)
      {
         country_mappings_.emplace(country.GetNumber(), mappingRule->second);
         return true;
      }
      return false;
   };

   DeferCountryAlways = [this](const vic3::Country& country) -> bool {
      deferred_map_countries_.push_back(&country);
      return true;
   };

   AddCountryWithVicId = [this](const vic3::Country& country) -> bool {
      const auto& vic3_tag = country.GetTag();
      if (vic3_tag.length() == 3 && used_hoi4_tags_.emplace(vic3_tag).second)
      {
         country_mappings_.emplace(country.GetNumber(), vic3_tag);
         return true;
      }
      return false;
   };

   AddCountryWithZ = [this](const vic3::Country& country) -> bool {
      std::string possible_hoi4_tag;
      do
      {
         char prefix = tag_prefix_;
         int tens = tag_suffix_ / 10;
         int ones = tag_suffix_ % 10;

         possible_hoi4_tag = fmt::format("{}{}{}", prefix, tens, ones);
         ++tag_suffix_;
         if (tag_suffix_ > 99)
         {
            tag_suffix_ = 0;
            --tag_prefix_;
         }
      } while (used_hoi4_tags_.contains(possible_hoi4_tag));
      country_mappings_.emplace(country.GetNumber(), possible_hoi4_tag);
      used_hoi4_tags_.emplace(possible_hoi4_tag);
      return true;
   };
}

// Attempt to name (or defer naming of) a country. Only the first successful strategy will be used.
void mappers::CountryMappingCreator::ExecuteStrategiesForCountry(const vic3::Country& country,
    const std::vector<CountryStrategyFn>&& strategies)
{
   static_cast<void>(std::any_of(strategies.begin(), strategies.end(), [&country](CountryStrategyFn fn) {
      return fn(country);
   }));
}



// Where the logic happens
std::map<int, std::string> mappers::CountryMappingCreator::AssignTags(auto countries)
{
   country_mappings_.clear();
   deferred_map_countries_.clear();
   tag_prefix_ = 'Z';
   tag_suffix_ = 0;

   for (const auto& country: countries)
   {
      ExecuteStrategiesForCountry(country, {DeferCountryWithCivilWar, AddCountryWithRule, DeferCountryAlways});
   }

   // after we got the initial rule countries, try again via vicId and then Znn
   std::vector<const vic3::Country*> currentDeferredCountries(std::move(deferred_map_countries_));
   deferred_map_countries_.clear();
   for (const auto& country: currentDeferredCountries)
   {
      ExecuteStrategiesForCountry(*country, {DeferCountryWithCivilWar, AddCountryWithVicId, AddCountryWithZ});
   }
   // finally try the civil war countries
   currentDeferredCountries = std::move(deferred_map_countries_);
   deferred_map_countries_.clear();
   for (const auto& country: currentDeferredCountries)
   {
      ExecuteStrategiesForCountry(*country, {AddCountryWithRule, AddCountryWithVicId, AddCountryWithZ});
   }

   return country_mappings_;
}



mappers::CountryMapper mappers::CreateCountryMappings(std::string_view country_mappings_file,
    const std::map<int, vic3::Country>& countries)
{
   CountryMappingCreator mappingCreator(country_mappings_file);
   return CountryMapper(mappingCreator.AssignTags(countries | std::views::values));
}