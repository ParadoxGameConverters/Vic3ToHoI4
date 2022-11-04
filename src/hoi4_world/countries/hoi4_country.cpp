#include "src/hoi4_world/countries/hoi4_country.h"



hoi4::Country::Country(const vic3::Country& source_country, const mappers::CountryMapper& country_mapper)
{
   tag_ = country_mapper.GetHoiTag(source_country.GetTag());
}