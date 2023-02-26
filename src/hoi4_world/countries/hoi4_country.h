#ifndef SRC_HOI4WORLD_COUNTRIES_HOI4COUNTRY_H
#define SRC_HOI4WORLD_COUNTRIES_HOI4COUNTRY_H



#include <string>

#include "external/commonItems/Color.h"
#include "src/hoi4_world/technology/technologies.h"
#include "src/mappers/country/country_mapper.h"



namespace hoi4
{

struct CountryOptions
{
   std::string tag;
   commonItems::Color color;
   std::optional<int> capital_state;
   Technologies technologies;
};

class Country
{
  public:
   explicit Country(CountryOptions country_options):
       tag_(std::move(country_options.tag)),
       color_(std::move(country_options.color)),
       capital_state_(std::move(country_options.capital_state)),
       technologies_(std::move(country_options.technologies))
   {
   }

   [[nodiscard]] const std::string& GetTag() const { return tag_; }
   [[nodiscard]] const commonItems::Color& GetColor() const { return color_; }
   [[nodiscard]] const std::optional<int>& GetCapitalState() const { return capital_state_; }
   [[nodiscard]] const Technologies& GetTechnologies() const { return technologies_; }

   std::strong_ordering operator<=>(const Country&) const = default;

  private:
   std::string tag_;
   commonItems::Color color_;
   std::optional<int> capital_state_;
   Technologies technologies_;
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_COUNTRIES_HOI4COUNTRY_H