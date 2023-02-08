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
   std::optional<int> capital_state;
};

class Country
{
  public:
   explicit Country(CountryOptions country_options):
       tag_(country_options.tag),
       color_(country_options.color),
       capital_state_(country_options.capital_state)
   {
   }

   [[nodiscard]] const std::string& GetTag() const { return tag_; }
   [[nodiscard]] const commonItems::Color& GetColor() const { return color_; }
   [[nodiscard]] const std::optional<int>& GetCapitalState() const { return capital_state_; }

   bool operator==(const Country&) const = default;

  private:
   std::string tag_;
   commonItems::Color color_;
   std::optional<int> capital_state_;
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_COUNTRIES_HOI4COUNTRY_H