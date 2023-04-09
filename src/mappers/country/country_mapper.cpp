#include "src/mappers/country/country_mapper.h"



std::optional<std::string> mappers::CountryMapper::GetHoiTag(const int& vic_number) const
{
   if (const auto mapping = country_mappings_.find(vic_number); mapping != country_mappings_.end())
   {
      return mapping->second;
   }

   return std::nullopt;
}