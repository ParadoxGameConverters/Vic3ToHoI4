#ifndef SRC_HOI4WORLD_COUNTRIES_COUNTRY_H
#define SRC_HOI4WORLD_COUNTRIES_COUNTRY_H



#include <string>

#include "src/mappers/country/country_mapper.h"
#include "src/vic3_world/countries/country.h"



namespace hoi4
{

class Country
{
  public:
   explicit Country(const vic3::Country& src_country, const mappers::CountryMapper& country_mapper);

   [[nodiscard]] const std::string& GetTag() const { return tag_; }

  private:
   std::string tag_;
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_COUNTRIES_COUNTRY_H