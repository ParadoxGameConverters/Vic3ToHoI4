#ifndef SRC_VIC3WORLD_COUNTRIES_VIC3COUNTRY_H
#define SRC_VIC3WORLD_COUNTRIES_VIC3COUNTRY_H



#include <string>

#include "external/commonItems/Color.h"
#include "external/commonItems/Date.h"


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
   std::set<int> primary_culture_ids;
   std::set<std::string> primary_cultures;
   std::optional<date> last_election;
   int head_of_state_id = 0;
   std::vector<int> character_ids;
   std::vector<int> ig_ids;
   std::set<int> puppets;
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
       active_laws_(std::move(options.active_laws)),
       primary_cultures_(std::move(options.primary_cultures)),
       primary_culture_ids_(std::move(options.primary_culture_ids)),
       last_election_(options.last_election),
       head_of_state_id_(options.head_of_state_id),
       character_ids_(std::move(options.character_ids)),
       ig_ids_(std::move(options.ig_ids)),
       puppets_(std::move(options.puppets))
   {
   }

   [[nodiscard]] int GetNumber() const { return number_; }
   [[nodiscard]] const std::string& GetTag() const { return tag_; }
   [[nodiscard]] const commonItems::Color& GetColor() const { return color_; }
   [[nodiscard]] const std::optional<int>& GetCapitalState() const { return capital_state_; }
   [[nodiscard]] bool IsDecentralized() const { return country_type_ == "decentralized"; }
   [[nodiscard]] const std::set<std::string>& GetActiveLaws() const { return active_laws_; }
   [[nodiscard]] const std::set<std::string>& GetPrimaryCultures() const { return primary_cultures_; }
   [[nodiscard]] const std::set<int>& GetPrimaryCultureIds() const { return primary_culture_ids_; }
   [[nodiscard]] const std::optional<date>& GetLastElection() const { return last_election_; }
   [[nodiscard]] int GetHeadOfStateId() const { return head_of_state_id_; }
   [[nodiscard]] const std::vector<int>& GetCharacterIds() const { return character_ids_; }
   [[nodiscard]] const std::vector<int>& GetInterestGroupIds() const { return ig_ids_; }
   [[nodiscard]] const std::set<int>& GetPuppets() const { return puppets_; }

   void SetActiveLaws(std::set<std::string> active_laws) { active_laws_ = std::move(active_laws); }
   void SetLastElection(date last_election) { last_election_ = last_election; }
   void AddPrimaryCulture(const std::string& culture) { primary_cultures_.emplace(culture); }
   void SetCharacterIds(const std::vector<int>& character_ids) { character_ids_ = character_ids; }
   void AddInterestGroupId(int ig_id) { ig_ids_.push_back(ig_id); }
   void AddPuppet(int puppet) { puppets_.insert(puppet); }

   /// <summary>
   /// Set the world that this country is a part of. Used for stats stored in global lists (eg, country rank). Set when inserting this country into a world.
   /// </summary>
   /// <param name="world">world</param>
   void SetWorld(const World& world) { world_.emplace(world); }
   std::set<std::string> GetAcquiredTechnologies()
   {
      if (!world_.has_value())
      {
         throw std::exception(
             "GetAcquiredTechnologies called before country was inserted into a world. This is a code logic error and "
             "should be fixed.");
      }
      
      const auto allTechs = world_->GetAcquiredTechnologies();
      const auto maybeVal = allTechs.find(this->GetNumber());
      if (maybeVal != allTechs.end())
      {
         return *maybeVal;
      }
      else
      {
         return {};
      }
   }

   bool operator==(const Country&) const = default;

  private:
   int number_ = 0;
   std::string tag_;
   commonItems::Color color_;
   std::optional<int> capital_state_;
   std::string country_type_;
   std::set<std::string> active_laws_;
   std::set<std::string> primary_cultures_;
   std::set<int> primary_culture_ids_;  // Resolve to culture name before HoI
   std::optional<date> last_election_;
   int head_of_state_id_ = 0;
   std::vector<int> character_ids_;
   std::vector<int> ig_ids_;
   std::set<int> puppets_;

   /// <summary>
   /// world pointer for country stats that are kept in global lists (eg, ranking).
   /// Set when country is added to a world.
   /// </summary>
   std::optional<World&> world_;
};

}  // namespace vic3



#endif  // SRC_VIC3WORLD_COUNTRIES_VIC3COUNTRY_H