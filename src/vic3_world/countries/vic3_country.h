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
   std::set<std::string> active_laws;
};


class Country
{
  public:
   explicit Country(CountryOptions options):
       number_(options.number),
       tag_(std::move(options.tag)),
       color_(options.color),
       capital_state_(options.capital_state),
       country_type_(std::move(options.country_type)),
       active_laws_(std::move(options.active_laws))
   {
   }

   [[nodiscard]] int GetNumber() const { return number_; }
   [[nodiscard]] const std::string& GetTag() const { return tag_; }
   [[nodiscard]] const commonItems::Color& GetColor() const { return color_; }
   [[nodiscard]] const std::optional<int>& GetCapitalState() const { return capital_state_; }
   [[nodiscard]] bool IsDecentralized() const { return country_type_ == "decentralized"; }
   [[nodiscard]] const std::set<std::string>& GetActiveLaws() const { return active_laws_; }

   void SetActiveLaws(std::set<std::string> active_laws) { active_laws_ = std::move(active_laws); }
   void SetLastElection(date last_election) { last_election_ = last_election; }

   bool operator==(const Country&) const = default;

  private:
   int number_ = 0;
   std::string tag_;
   commonItems::Color color_;
   std::optional<int> capital_state_;
   std::string country_type_;
   std::set<std::string> active_laws_;
   std::optional<date> last_election_;
};

}  // namespace vic3



#endif  // SRC_VIC3WORLD_COUNTRIES_VIC3COUNTRY_H