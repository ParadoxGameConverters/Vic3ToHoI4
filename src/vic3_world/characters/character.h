// ideology

// traits

#ifndef SRC_VIC3WORLD_CHARACTERS_CHARACTER_H
#define SRC_VIC3WORLD_CHARACTERS_CHARACTER_H



#include <istream>



namespace vic3
{
struct CharacterOptions
{
   int id;
   std::string first_name;
   std::string last_name;
   int culture_id;
   std::string culture;
   std::set<std::string> roles;
   int rank;
   std::string ideology;
   std::set<std::string> traits;
   std::string origin_tag;
};

class Character
{
  public:
   explicit Character(CharacterOptions options):
       id_(options.id),
       first_name_(std::move(options.first_name)),
       last_name_(std::move(options.last_name)),
       culture_id_(options.culture_id),
       roles_(std::move(options.roles)),
       rank_(options.rank),
       ideology_(std::move(options.ideology)),
       traits_(std::move(options.traits)),
       origin_tag_(std::move(options.origin_tag))
   {
   }

   [[nodiscard]] const int GetId() const { return id_; }
   [[nodiscard]] const std::string& GetFirstName() const { return first_name_; }
   [[nodiscard]] const std::string& GetLastName() const { return last_name_; }

   [[nodiscard]] const int GetCultureId() const { return culture_id_; }
   [[nodiscard]] const std::string& GetCulture() const { return culture_; }
   [[nodiscard]] const std::set<std::string>& GetRoles() const { return roles_; }
   [[nodiscard]] const int GetRank() const { return rank_; }
   [[nodiscard]] const std::string& GetIdeology() const { return ideology_; }
   [[nodiscard]] const std::set<std::string>& GetTraits() const { return traits_; }
   [[nodiscard]] const std::string& GetOriginTag() const { return origin_tag_; }

   void SetHomeTag(std::string tag) { origin_tag_ = std::move(tag); }

   std::partial_ordering operator<=>(const Character&) const = default;

  private:
   int id_;
   std::string first_name_;
   std::string last_name_;

   int culture_id_;
   std::string culture_;
   std::set<std::string> roles_;
   int rank_;
   std::string ideology_;
   std::set<std::string> traits_;
   std::string origin_tag_;  // Where an agitator was exiled from
};
}  // namespace vic3



#endif  // SRC_VIC3WORLD_CHARACTERS_CHARACTER_H