#include "src/hoi4_world/countries/country.h"



hoi4::Country::Country(const vic3::Country& src_country, const mappers::CountryMapper& country_mapper)
{
   tag_ = country_mapper.GetHoiTag(src_country.GetTag());
}