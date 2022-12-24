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
   explicit CountryMapper(std::map<std::string, std::string> country_mappings):
       country_mappings_(std::move(country_mappings))
   {
   }

   [[nodiscard]] std::optional<std::string> GetHoiTag(const std::string& vic_tag) const;

  private:
   std::map<std::string, std::string> country_mappings_;
};

}  // namespace mappers



#endif  // SRC_MAPPERS_COUNTRY_COUNTRYMAPPER_H