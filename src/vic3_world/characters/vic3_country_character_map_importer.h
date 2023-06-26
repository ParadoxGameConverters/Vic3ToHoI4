#ifndef SRC_VIC3WORLD_CHARACTERS_VIC3COUNTRYCHARACTERMAPIMPORTER_H
#define SRC_VIC3WORLD_CHARACTERS_VIC3COUNTRYCHARACTERMAPIMPORTER_H



#include <istream>


namespace vic3
{

std::map<int, std::vector<int>> ImportCountryCharacterMap(std::istream& input_stream);

}  // namespace vic3



#endif  // SRC_VIC3WORLD_CHARACTERS_VIC3COUNTRYCHARACTERMAPIMPORTER_H