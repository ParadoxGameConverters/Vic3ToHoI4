#ifndef SRC_MAPPERS_COUNTRY_COUNTRYMAPPER_H
#define SRC_MAPPERS_COUNTRY_COUNTRYMAPPER_H



#include <map>
#include <optional>
#include <string>



namespace mappers
{

class CountryMapper
{
  public:
   CountryMapper() = default;
   explicit CountryMapper(std::map<int, std::string>&& country_mappings): country_mappings_(std::move(country_mappings))
   {
   }
   explicit CountryMapper(const std::map<int, std::string>& country_mappings): country_mappings_(country_mappings) {}

   [[nodiscard]] std::optional<std::string> GetHoiTag(const int& vic_number) const;

   [[nodiscard]] const std::map<int, std::string>& GetCountryMappings() const { return country_mappings_; }

  private:
   std::map<int, std::string> country_mappings_;
};

}  // namespace mappers



#endif  // SRC_MAPPERS_COUNTRY_COUNTRYMAPPER_H