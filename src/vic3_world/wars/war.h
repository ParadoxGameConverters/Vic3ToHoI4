#ifndef SRC_VIC3WORLD_WARS_WAR_H
#define SRC_VIC3WORLD_WARS_WAR_H



#include <optional>
#include <set>
#include <string>



namespace vic3
{

struct WarOptions
{
   int original_attacker;
   std::set<int> attackers;
   int original_defender;
   std::set<int> defenders;
};


class War
{
  public:
   War(const WarOptions& options):
       original_attacker_(options.original_attacker),
       attackers_(options.attackers),
       original_defender_(options.original_defender),
       defenders_(options.defenders)
   {
   }

   [[nodiscard]] int GetOriginalAttacker() const { return original_attacker_; }
   [[nodiscard]] const std::set<int>& GetAttackers() const { return attackers_; }
   [[nodiscard]] int GetOriginalDefender() const { return original_defender_; }
   [[nodiscard]] const std::set<int>& GetDefenders() const { return defenders_; }

  private:
   int original_attacker_;
   std::set<int> attackers_;
   int original_defender_;
   std::set<int> defenders_;
};


}  // namespace vic3



#endif  // SRC_VIC3WORLD_WARS_WAR_H