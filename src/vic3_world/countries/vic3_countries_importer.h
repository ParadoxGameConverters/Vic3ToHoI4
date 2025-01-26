#ifndef SRC_VIC3WORLD_COUNTRIES_VIC3COUNTRIESIMPORTER_H
#define SRC_VIC3WORLD_COUNTRIES_VIC3COUNTRIESIMPORTER_H



#include <external/commonItems/Parser.h>

#include <istream>
#include <map>

#include "src/vic3_world/countries/vic3_country.h"
#include "src/vic3_world/countries/vic3_country_importer.h"



namespace vic3
{

[[nodiscard]] std::map<int, Country> ImportCountries(const std::map<std::string, commonItems::Color>& color_definitions,
    std::istream& input_stream);

}  // namespace vic3



#endif  // SRC_VIC3WORLD_COUNTRIES_VIC3COUNTRIESIMPORTER_H