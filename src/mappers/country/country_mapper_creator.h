#ifndef SRC_MAPPERS_COUNTRY_COUNTRYMAPPERIMPORTER_H
#define SRC_MAPPERS_COUNTRY_COUNTRYMAPPERIMPORTER_H



#include <map>
#include <string_view>

#include "src/mappers/country/country_mapper.h"
#include "src/vic3_world/countries/vic3_country.h"



namespace mappers
{

[[nodiscard]] CountryMapper CreateCountryMappings(std::string_view country_mappings_file,
    const std::map<int, vic3::Country>& countries);


using CountryStrategyFn = std::function<bool(const vic3::Country&)>;


class CountryMappingCreator
{
  public:
   explicit CountryMappingCreator(const std::filesystem::path& country_mappings_file);

   std::map<int, std::string> AssignTags(auto countries);

  private:
   std::map<int, std::string> country_mappings_;
   std::map<std::string, std::string> country_mapping_rules_;
   std::vector<const vic3::Country*> deferred_map_countries_;

   std::set<std::string> used_hoi4_tags_;
   char tag_prefix_ = 'Z';
   int tag_suffix_ = 0;

   // Rebel countries should be deferred to give priority to non-rebel countries.
   CountryStrategyFn defer_country_with_civil_war_;

   // Countries with an existing rule should be converted.
   CountryStrategyFn add_country_with_rule_;

   // Countries should always be deferred.
   CountryStrategyFn defer_country_always_;

   // Countries should be added via their vic3 tag.
   CountryStrategyFn add_country_with_vic_id_;

   // Countries should be added with Znn naming scheme. This always succeeds.
   CountryStrategyFn add_country_with_z_;

   // Attempt to name (or defer naming of) a country. Only the first successful strategy will be used.
   void ExecuteStrategiesForCountry(const vic3::Country& country, const std::vector<CountryStrategyFn>&& strategies);
};

}  // namespace mappers



#endif  // SRC_MAPPERS_COUNTRY_COUNTRYMAPPER_H