#ifndef SRC_VIC3WORLD_COUNTRYRANKINGS_COUNTRYRANKINGS_H
#define SRC_VIC3WORLD_COUNTRYRANKINGS_COUNTRYRANKINGS_H



#include <set>



namespace vic3
{

class CountryRankings
{
  public:
   CountryRankings() = default;
   CountryRankings(std::set<int> great_powers, std::set<int> major_powers):
       great_powers_(std::move(great_powers)),
       major_powers_(std::move(major_powers))
   {
   }

   [[nodiscard]] const std::set<int>& GetGreatPowers() const { return great_powers_; }
   [[nodiscard]] const std::set<int>& GetMajorPowers() const { return major_powers_; }


  private:
   std::set<int> great_powers_;
   std::set<int> major_powers_;
};

}  // namespace vic3



#endif  // SRC_VIC3WORLD_COUNTRYRANKINGS_COUNTRYRANKINGS_H