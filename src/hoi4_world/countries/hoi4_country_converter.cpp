#include "src/hoi4_world/countries/hoi4_country_converter.h"



hoi4::Country hoi4::CountryConverter::ConvertCountry(const vic3::Country& source_country,
    const mappers::CountryMapper& country_mapper)
{
   return Country(country_mapper.GetHoiTag(source_country.GetTag()));
}