#ifndef SRC_MAPPERS_COUNTRY_COUNTRYMAPPER_H
#define SRC_MAPPERS_COUNTRY_COUNTRYMAPPER_H



#include <string>



namespace mappers
{

class CountryMapper
{
  public:
   CountryMapper() = default;

   // a quick hack so that we're calling a component we'll use a lot later, but not spend much time developing it now
   [[nodiscard]] std::string GetHoiTag(std::string_view vic_tag) const { return std::string(vic_tag); }
};

}  // namespace mappers



#endif  // SRC_MAPPERS_COUNTRY_COUNTRYMAPPER_H