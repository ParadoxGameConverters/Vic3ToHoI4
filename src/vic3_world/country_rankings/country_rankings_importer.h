#ifndef SRC_VIC3WORLD_COUNTRYRANKINGS_COUNTRYRANKINGSIMPORTER_H
#define SRC_VIC3WORLD_COUNTRYRANKINGS_COUNTRYRANKINGSIMPORTER_H



#include <istream>

#include "src/vic3_world/country_rankings/country_rankings.h"



namespace vic3
{

[[nodiscard]] CountryRankings ImportCountryRankings(std::istream& input_stream);

}  // namespace vic3



#endif  // SRC_VIC3WORLD_COUNTRYRANKINGS_COUNTRYRANKINGSIMPORTER_H