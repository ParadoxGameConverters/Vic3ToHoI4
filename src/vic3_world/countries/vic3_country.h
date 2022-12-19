#ifndef SRC_VIC3WORLD_COUNTRIES_VIC3COUNTRY_H
#define SRC_VIC3WORLD_COUNTRIES_VIC3COUNTRY_H



#include <string>

#include "external/commonItems/Color.h"



namespace vic3
{

struct CountryOptions
{
   std::string tag;
   commonItems::Color color;
};


class Country
{
  public:
   explicit Country(CountryOptions options): tag_(std::move(options.tag)), color_(std::move(options.color)) {}

   [[nodiscard]] const std::string& GetTag() const { return tag_; }
   [[nodiscard]] const commonItems::Color& GetColor() const { return color_; }

   bool operator==(const Country&) const = default;

  private:
   std::string tag_;
   commonItems::Color color_;
};

}  // namespace vic3



#endif  // SRC_VIC3WORLD_COUNTRIES_VIC3COUNTRY_H