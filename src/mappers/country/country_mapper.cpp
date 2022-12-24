#include "src/mappers/country/country_mapper.h"


std::optional<std::string> mappers::CountryMapper::GetHoiTag(const std::string& vic_tag) const
{
   if (const auto mapping = country_mappings_.find(vic_tag); mapping != country_mappings_.end())
   {
      return mapping->second;
   }

   return std::nullopt;
}