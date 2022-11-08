#ifndef SRC_VIC3WORLD_COUNTRIES_VIC3COUNTRY_H
#define SRC_VIC3WORLD_COUNTRIES_VIC3COUNTRY_H



#include <string>



namespace vic3
{

class Country
{
  public:
   explicit Country(std::string tag): tag_(std::move(tag)) {}

   [[nodiscard]] const std::string& GetTag() const { return tag_; }

   bool operator==(const Country&) const = default;

  private:
   std::string tag_;
};

}  // namespace vic3



#endif  // SRC_VIC3WORLD_COUNTRIES_VIC3COUNTRY_H