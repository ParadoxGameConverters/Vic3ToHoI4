#ifndef SRC_VIC3WORLD_CHARACTERS_VIC3CHARACTER_H
#define SRC_VIC3WORLD_CHARACTERS_VIC3CHARACTER_H



#include <istream>



namespace vic3
{
struct CharacterOptions
{
   int id = 0;
   std::string first_name;
   std::string last_name;
   int culture_id = 0;
   std::string culture;
   int ig_id = 0;
   std::set<std::string> roles;
   int rank = 0;
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
       culture_(options.culture),
       ig_id_(options.ig_id),
       roles_(std::move(options.roles)),
       rank_(options.rank),
       ideology_(std::move(options.ideology)),
       traits_(std::move(options.traits)),
       origin_tag_(std::move(options.origin_tag))
   {
   }

   [[nodiscard]] int GetId() const { return id_; }
   [[nodiscard]] const std::string& GetFirstName() const { return first_name_; }
   [[nodiscard]] const std::string& GetLastName() const { return last_name_; }

   [[nodiscard]] int GetCultureId() const { return culture_id_; }
   [[nodiscard]] const std::string& GetCulture() const { return culture_; }
   [[nodiscard]] int GetIgId() const { return rank_; }
   [[nodiscard]] const std::set<std::string>& GetRoles() const { return roles_; }
   [[nodiscard]] int GetRank() const { return rank_; }
   [[nodiscard]] const std::string& GetIdeology() const { return ideology_; }
   [[nodiscard]] const std::set<std::string>& GetTraits() const { return traits_; }
   [[nodiscard]] const std::string& GetOriginTag() const { return origin_tag_; }

   void SetHomeTag(std::string tag) { origin_tag_ = std::move(tag); }
   void SetIgId(int id) { ig_id_ = id; }

   std::partial_ordering operator<=>(const Character&) const = default;

  private:
   int id_ = 0;
   std::string first_name_;
   std::string last_name_;

   int culture_id_ = 0;
   std::string culture_;
   int ig_id_ = 0;
   std::set<std::string> roles_;
   int rank_ = 0;
   std::string ideology_;
   std::set<std::string> traits_;
   std::string origin_tag_;  // Where an agitator was exiled from
};
}  // namespace vic3



#endif  // SRC_VIC3WORLD_CHARACTERS_VIC3CHARACTER_H