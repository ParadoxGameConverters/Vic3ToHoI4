#include "src/hoi4_world/countries/hoi4_country_converter.h"



std::optional<hoi4::Country> hoi4::CountryConverter::ConvertCountry(const vic3::Country& source_country,
    const mappers::CountryMapper& country_mapper)
{
   const auto tag = country_mapper.GetHoiTag(source_country.GetTag());
   if (tag.has_value())
   {
      return Country({.tag = *tag, .color = source_country.GetColor()});
   }

   return std::nullopt;
}