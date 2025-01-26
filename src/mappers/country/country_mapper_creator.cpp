#include "src/mappers/country/country_mapper_creator.h"

#include <external/commonItems/Log.h>
#include <external/commonItems/Parser.h>
#include <external/commonItems/ParserHelpers.h>
#include <external/fmt/include/fmt/format.h>

#include <execution>
#include <queue>
#include <ranges>



using std::filesystem::path;



namespace
{

const std::set<std::string> kDisallowedTags{"CON", "PRN", "AUX", "NUL"};


std::map<std::string, std::string> ImportMappingRules(const path& country_mappings_file)
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
          if (!kDisallowedTags.contains(hoi4_tag))
          {
             country_mapping_rules.emplace(vic3_tag, hoi4_tag);
          }
          else
          {
             Log(LogLevel::Warning) << fmt::format("{} is a disallowed tag, skipping", hoi4_tag);
          }
       });

   country_mappings_parser.parseFile(country_mappings_file);

   return country_mapping_rules;
}


bool IsDynamicTag(std::string_view tag)
{
   return tag.size() == 3 && tag.starts_with('D') && std::isdigit(tag[1]) && std::isdigit(tag[2]);
}

}  // namespace


mappers::CountryMappingCreator::CountryMappingCreator(const path& country_mappings_file)
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
      const auto& mapping_rule = country_mapping_rules_.find(vic3_tag);
      if (mapping_rule != country_mapping_rules_.end() && used_hoi4_tags_.emplace(mapping_rule->second).second)
      {
         country_mappings_.emplace(country.GetNumber(), mapping_rule->second);
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
      if (IsDynamicTag(vic3_tag))
      {
         return false;
      }
      if (kDisallowedTags.contains(vic3_tag))
      {
         return false;
      }
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
         possible_hoi4_tag = fmt::format("{}{:0>2}", tag_prefix_, tag_suffix_);
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
   static_cast<void>(std::ranges::any_of(strategies, [&country](const CountryStrategyFn& fn) {
      return fn(country);
   }));
}


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
   std::vector<const vic3::Country*> current_deferred_countries(std::move(deferred_map_countries_));
   deferred_map_countries_.clear();
   for (const auto& country: current_deferred_countries)
   {
      ExecuteStrategiesForCountry(*country, {DeferCountryWithCivilWar, AddCountryWithVicId, AddCountryWithZ});
   }
   // finally try the civil war countries
   current_deferred_countries = std::move(deferred_map_countries_);
   deferred_map_countries_.clear();
   for (const auto& country: current_deferred_countries)
   {
      ExecuteStrategiesForCountry(*country, {AddCountryWithRule, AddCountryWithVicId, AddCountryWithZ});
   }

   return country_mappings_;
}


mappers::CountryMapper mappers::CreateCountryMappings(std::string_view country_mappings_file,
    const std::map<int, vic3::Country>& countries)
{
   CountryMappingCreator mapping_creator(country_mappings_file);
   return CountryMapper(mapping_creator.AssignTags(countries | std::views::values));
}