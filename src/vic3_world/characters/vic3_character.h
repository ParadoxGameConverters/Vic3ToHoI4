#ifndef SRC_VIC3WORLD_CHARACTERS_VIC3CHARACTER_H
#define SRC_VIC3WORLD_CHARACTERS_VIC3CHARACTER_H



#include <istream>
#include <optional>
#include <set>
#include <string>



namespace vic3
{
struct CharacterOptions
{
   int id = 0;
   std::string first_name;
   std::string last_name;
   int culture_id = 0;
   std::string culture;
   bool is_female = false;
   int ig_id = 0;
   std::set<std::string> roles;
   int rank = 0;
   std::string ideology;
   std::set<std::string> traits;
   std::string origin_tag;
   std::optional<int> origin_country_id;
   std::optional<int> formation_id;
};

class Character
{
  public:
   explicit Character(CharacterOptions options):
       id_(options.id),
       first_name_(std::move(options.first_name)),
       last_name_(std::move(options.last_name)),
       culture_id_(options.culture_id),
       culture_(options.culture),
       is_female_(options.is_female),
       ig_id_(options.ig_id),
       roles_(std::move(options.roles)),
       rank_(options.rank),
       ideology_(std::move(options.ideology)),
       traits_(std::move(options.traits)),
       origin_tag_(std::move(options.origin_tag)),
       origin_country_id_(options.origin_country_id),
       formation_id_(options.formation_id)
   {
   }

   [[nodiscard]] int GetId() const { return id_; }
   [[nodiscard]] const std::string& GetFirstName() const { return first_name_; }
   [[nodiscard]] const std::string& GetLastName() const { return last_name_; }

   [[nodiscard]] int GetCultureId() const { return culture_id_; }
   [[nodiscard]] const std::string& GetCulture() const { return culture_; }
   [[nodiscard]] bool IsFemale() const { return is_female_; }
   [[nodiscard]] int GetIgId() const { return rank_; }
   [[nodiscard]] const std::set<std::string>& GetRoles() const { return roles_; }
   [[nodiscard]] int GetRank() const { return rank_; }
   [[nodiscard]] const std::string& GetIdeology() const { return ideology_; }
   [[nodiscard]] const std::set<std::string>& GetTraits() const { return traits_; }
   [[nodiscard]] const std::string& GetOriginTag() const { return origin_tag_; }
   [[nodiscard]] const std::optional<int>& GetOriginCountryId() const { return origin_country_id_; }
   [[nodiscard]] bool IsCommander() const { return formation_id_.has_value(); }


   void SetCulture(std::string culture) { culture_ = std::move(culture); }
   void SetHomeTag(std::string tag) { origin_tag_ = std::move(tag); }
   void SetOriginCountryId(int id) { origin_country_id_ = id; }
   void SetIgId(const int id) { ig_id_ = id; }
   void SetCommander() { formation_id_ = 0; }  // For backwards compatibility with pre 1.5 Vic

   std::partial_ordering operator<=>(const Character&) const = default;

  private:
   int id_ = 0;
   std::string first_name_;
   std::string last_name_;

   int culture_id_ = 0;
   std::string culture_;
   bool is_female_ = false;
   int ig_id_ = 0;
   std::set<std::string> roles_;
   int rank_ = 0;
   std::string ideology_;
   std::set<std::string> traits_;
   std::string origin_tag_;  // Where an agitator was exiled from, resolve to ID before HoI
   std::optional<int> origin_country_id_;
   std::optional<int> formation_id_;  // Commanders with formations are actively hired to lead boats/troops
};
}  // namespace vic3



#endif  // SRC_VIC3WORLD_CHARACTERS_VIC3CHARACTER_H