#ifndef SRC_HOI4WORLD_COUNTRIES_HOI4COUNTRY_H
#define SRC_HOI4WORLD_COUNTRIES_HOI4COUNTRY_H



#include <string>

#include "external/commonItems/Color.h"
#include "src/mappers/country/country_mapper.h"
#include "src/vic3_world/countries/vic3_country.h"



namespace hoi4
{

struct CountryOptions
{
   std::string tag;
   commonItems::Color color;
};

class Country
{
  public:
   explicit Country(CountryOptions country_options): tag_(country_options.tag), color_(std::move(country_options.color))
   {
   }

   [[nodiscard]] const std::string& GetTag() const { return tag_; }
   [[nodiscard]] const commonItems::Color& GetColor() const { return color_; }

   bool operator==(const Country&) const = default;

  private:
   std::string tag_;
   commonItems::Color color_;
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_COUNTRIES_HOI4COUNTRY_H