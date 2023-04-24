#ifndef SRC_VIC3WORLD_COUNTRIES_VIC3COUNTRY_H
#define SRC_VIC3WORLD_COUNTRIES_VIC3COUNTRY_H



#include <string>

#include "external/commonItems/Color.h"



namespace vic3
{

struct CountryOptions
{
   int number = 0;
   std::string tag;
   commonItems::Color color;
   std::optional<int> capital_state;
   std::string country_type;
};


class Country
{
  public:
   Country() = default;
   explicit Country(CountryOptions options):
       number_(options.number),
       tag_(std::move(options.tag)),
       color_(options.color),
       capital_state_(options.capital_state),
       country_type_(std::move(options.country_type))
   {
   }

   [[nodiscard]] int GetNumber() const { return number_; }
   [[nodiscard]] const std::string& GetTag() const { return tag_; }
   [[nodiscard]] const commonItems::Color& GetColor() const { return color_; }
   [[nodiscard]] const std::optional<int>& GetCapitalState() const { return capital_state_; }
   [[nodiscard]] bool IsDecentralized() const { return country_type_ == "decentralized"; }

   bool operator==(const Country&) const = default;

  private:
   int number_ = 0;
   std::string tag_;
   commonItems::Color color_;
   std::optional<int> capital_state_;
   std::string country_type_;
};

}  // namespace vic3



#endif  // SRC_VIC3WORLD_COUNTRIES_VIC3COUNTRY_H