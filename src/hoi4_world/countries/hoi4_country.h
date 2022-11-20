#ifndef SRC_HOI4WORLD_COUNTRIES_HOI4COUNTRY_H
#define SRC_HOI4WORLD_COUNTRIES_HOI4COUNTRY_H



#include <string>

#include "src/mappers/country/country_mapper.h"
#include "src/vic3_world/countries/vic3_country.h"



namespace hoi4
{

class Country
{
  public:
   explicit Country(std::string_view tag): tag_(tag) {}

   [[nodiscard]] const std::string& GetTag() const { return tag_; }

   bool operator==(const Country&) const = default;

  private:
   std::string tag_;
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_COUNTRIES_HOI4COUNTRY_H