#ifndef SRC_VIC3WORLD_COUNTRYRANKINGS_COUNTRYRANKINGS_H
#define SRC_VIC3WORLD_COUNTRYRANKINGS_COUNTRYRANKINGS_H



#include <set>



namespace vic3
{

class CountryRankings
{
  public:
   CountryRankings() = default;
   CountryRankings(std::set<int> great_powers, std::set<int> major_powers, std::map<int, int> scored_countries):
       great_powers_(std::move(great_powers)),
       major_powers_(std::move(major_powers)),
       scored_countries_(std::move(scored_countries))
   {
   }

   [[nodiscard]] const std::set<int>& GetGreatPowers() const { return great_powers_; }
   [[nodiscard]] const std::set<int>& GetMajorPowers() const { return major_powers_; }
   [[nodiscard]] const std::map<int, int>& GetScoredCountries() const { return scored_countries_; }

  private:
   std::set<int> great_powers_;
   std::set<int> major_powers_;
   std::map<int, int> scored_countries_;
};

}  // namespace vic3



#endif  // SRC_VIC3WORLD_COUNTRYRANKINGS_COUNTRYRANKINGS_H