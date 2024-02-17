#ifndef SRC_VIC3WORLD_COUNTRIES_VIC3COUNTRY_H
#define SRC_VIC3WORLD_COUNTRIES_VIC3COUNTRY_H



#include <string>

#include "external/commonItems/Color.h"
#include "external/commonItems/Date.h"
#include "src/vic3_world/institutions/institution.h"
#include "src/vic3_world/military/military_formation.h"



namespace vic3
{

// need to extra-forward-declare this because World uses Country, and Country uses World
class World;


/// <summary>
/// tax level, salary level, etc.
/// </summary>
enum class BudgetLevel
{
   VeryLow,
   Low,
   Medium,
   High,
   VeryHigh
};


struct CountryOptions
{
   int number = 0;
   std::string tag;
   bool is_dead;
   std::optional<std::string> dynamic_name;
   std::optional<std::string> dynamic_adjective;
   bool use_overlord_prefix = false;
   commonItems::Color color;
   std::optional<int> capital_state;
   std::string country_type;
   bool is_civil_war = false;
   std::set<std::string> active_laws = {};
   std::set<int> primary_culture_ids;
   std::set<std::string> primary_cultures;
   std::optional<date> last_election;
   std::int64_t head_of_state_id = 0;
   std::vector<int> character_ids;
   std::vector<int> ig_ids;
   std::set<int> puppets;
   std::optional<int> overlord;
   int legitimacy = 0;
   BudgetLevel tax_level;
   BudgetLevel salary_level;
   BudgetLevel mil_salary_level;
   std::map<int, MilitaryFormation> army_formations;
   std::map<int, MilitaryFormation> navy_formations;
};


enum class RankCategory
{
   GreatPower,
   MajorPower,
   MinorPower,          // called "unrecognized regional power" if unrecognized
   InsignificantPower,  // called "unrecognized power" if unrecognized
   DecentralizedPower
};


class Country
{
  public:
   explicit Country(CountryOptions options):
       number_(options.number),
       tag_(std::move(options.tag)),
       is_dead_(options.is_dead),
       dynamic_name_(std::move(options.dynamic_name)),
       dynamic_adjective_(std::move(options.dynamic_adjective)),
       use_overlord_prefix_(options.use_overlord_prefix),
       color_(options.color),
       capital_state_(options.capital_state),
       country_type_(std::move(options.country_type)),
       is_civil_war_(options.is_civil_war),
       active_laws_(std::move(options.active_laws)),
       primary_cultures_(std::move(options.primary_cultures)),
       primary_culture_ids_(std::move(options.primary_culture_ids)),
       last_election_(options.last_election),
       head_of_state_id_(options.head_of_state_id),
       character_ids_(std::move(options.character_ids)),
       ig_ids_(std::move(options.ig_ids)),
       puppets_(std::move(options.puppets)),
       overlord_(options.overlord),
       legitimacy_(options.legitimacy),
       tax_level_(options.tax_level),
       salary_level_(options.salary_level),
       mil_salary_level_(options.mil_salary_level),
       army_formations_(options.army_formations),
       navy_formations_(options.navy_formations)
   {
   }

   [[nodiscard]] int GetNumber() const { return number_; }
   [[nodiscard]] const std::string& GetTag() const { return tag_; }
   [[nodiscard]] bool IsDead() const { return is_dead_; }
   [[nodiscard]] const std::optional<std::string>& GetDynamicName() const { return dynamic_name_; }
   [[nodiscard]] const std::optional<std::string>& GetDynamicAdjective() const { return dynamic_adjective_; }
   [[nodiscard]] bool GetUseOverlordPrefix() const { return use_overlord_prefix_; }
   [[nodiscard]] const commonItems::Color& GetColor() const { return color_; }
   [[nodiscard]] const std::optional<int>& GetCapitalState() const { return capital_state_; }
   [[nodiscard]] bool IsDecentralized() const { return country_type_ == "decentralized"; }
   /// Is the country a recognized or colonial nation
   [[nodiscard]] bool IsRecognized() const { return !IsDecentralized() && country_type_ != "unrecognized"; }
   // are these rebels
   [[nodiscard]] bool IsCivilWarCountry() const { return is_civil_war_; }
   [[nodiscard]] const std::set<std::string>& GetActiveLaws() const { return active_laws_; }
   [[nodiscard]] const std::set<std::string>& GetPrimaryCultures() const { return primary_cultures_; }
   [[nodiscard]] const std::set<int>& GetPrimaryCultureIds() const { return primary_culture_ids_; }
   [[nodiscard]] const std::optional<date>& GetLastElection() const { return last_election_; }
   [[nodiscard]] std::int64_t GetHeadOfStateId() const { return head_of_state_id_; }
   [[nodiscard]] const std::vector<int>& GetCharacterIds() const { return character_ids_; }
   [[nodiscard]] const std::vector<int>& GetInterestGroupIds() const { return ig_ids_; }
   [[nodiscard]] const std::set<int>& GetPuppets() const { return puppets_; }
   [[nodiscard]] const std::optional<int>& GetOverlord() const { return overlord_; }
   [[nodiscard]] int GetLegitimacy() const { return legitimacy_; }
   [[nodiscard]] BudgetLevel GetTaxLevel() const { return tax_level_; }
   [[nodiscard]] BudgetLevel GetGovernmentSalaryLevel() const { return salary_level_; }
   [[nodiscard]] BudgetLevel GetMilitarySalaryLevel() const { return mil_salary_level_; }
   [[nodiscard]] const std::map<int, MilitaryFormation>& GetArmyFormations() const { return army_formations_; }
   [[nodiscard]] const std::map<int, MilitaryFormation>& GetNavyFormations() const { return navy_formations_; }

   void SetColor(commonItems::Color color) { color_ = std::move(color); }
   void SetActiveLaws(std::set<std::string> active_laws) { active_laws_ = std::move(active_laws); }
   void SetLastElection(date last_election) { last_election_ = last_election; }
   void AddPrimaryCulture(const std::string& culture) { primary_cultures_.emplace(culture); }
   void SetCharacterIds(const std::vector<int>& character_ids) { character_ids_ = character_ids; }
   void AddInterestGroupId(int ig_id) { ig_ids_.push_back(ig_id); }
   void AddPuppet(int puppet) { puppets_.insert(puppet); }
   void AddOverlord(int overlord) { overlord_ = overlord; }
   void SetArmyFormations(const std::map<int, MilitaryFormation>& military_formations)
   {
      army_formations_ = military_formations;
   }
   void SetNavyFormations(const std::map<int, MilitaryFormation>& military_formations)
   {
      navy_formations_ = military_formations;
   }

   [[nodiscard]] std::set<std::string> GetAcquiredTechnologies(const vic3::World& world) const;
   [[nodiscard]] RankCategory GetCountryRankCategory(const vic3::World& world) const;
   [[nodiscard]] std::vector<Institution> GetInstitutions(const vic3::World& world) const;

   bool operator==(const Country&) const = default;

  private:
   int number_ = 0;
   std::string tag_;
   bool is_dead_;
   std::optional<std::string> dynamic_name_;
   std::optional<std::string> dynamic_adjective_;
   bool use_overlord_prefix_ = false;
   commonItems::Color color_;
   std::optional<int> capital_state_;
   std::string country_type_;
   bool is_civil_war_;
   std::set<std::string> active_laws_;
   std::set<std::string> primary_cultures_;
   std::set<int> primary_culture_ids_;  // Resolve to culture name before HoI
   std::optional<date> last_election_;
   std::int64_t head_of_state_id_ = 0;
   std::vector<int> character_ids_;
   std::vector<int> ig_ids_;
   std::set<int> puppets_;
   std::optional<int> overlord_;
   int legitimacy_;
   BudgetLevel tax_level_;
   BudgetLevel salary_level_;
   BudgetLevel mil_salary_level_;
   std::map<int, MilitaryFormation> army_formations_;
   std::map<int, MilitaryFormation> navy_formations_;
};

}  // namespace vic3



#endif  // SRC_VIC3WORLD_COUNTRIES_VIC3COUNTRY_H