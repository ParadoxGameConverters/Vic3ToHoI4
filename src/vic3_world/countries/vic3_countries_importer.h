#ifndef SRC_VIC3WORLD_COUNTRIES_VIC3COUNTRIESIMPORTER_H
#define SRC_VIC3WORLD_COUNTRIES_VIC3COUNTRIESIMPORTER_H



#include <istream>
#include <map>

#include "external/commonItems/Parser.h"
#include "src/vic3_world/countries/vic3_country.h"
#include "src/vic3_world/countries/vic3_country_importer.h"



namespace vic3
{

class CountriesImporter
{
  public:
   explicit CountriesImporter(bool debug);

   [[nodiscard]] std::map<int, Country> ImportCountries(std::istream& input_stream);

  private:
   commonItems::parser countries_parser_;
   commonItems::parser database_parser_;
   CountryImporter country_importer_;

   std::map<int, Country> countries_;
};


}  // namespace vic3



#endif  // SRC_VIC3WORLD_COUNTRIES_VIC3COUNTRIESIMPORTER_H