#ifndef SRC_VIC3WORLD_COUNTRIES_VIC3COUNTRYIMPORTER_H
#define SRC_VIC3WORLD_COUNTRIES_VIC3COUNTRYIMPORTER_H



#include <istream>
#include <string>

#include "external/commonItems/Parser.h"
#include "src/vic3_world/countries/vic3_country.h"



namespace vic3
{

class CountryImporter
{
  public:
   CountryImporter();

   [[nodiscard]] std::optional<Country> ImportCountry(int number,
       std::istream& input_stream,
       const std::map<std::string, commonItems::Color>& color_definitions);

  private:
   commonItems::parser country_parser_;
   commonItems::parser counters_parser_;

   CountryOptions options_;

   bool is_dead_ = false;  // Country marked as invalid data by game, but not yet cleaned up
};

}  // namespace vic3



#endif  // SRC_VIC3WORLD_COUNTRIES_VIC3COUNTRYIMPORTER_H