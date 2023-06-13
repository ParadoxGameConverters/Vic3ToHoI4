// ideology

// traits

#ifndef SRC_VIC3WORLD_CHARACTERS_CHARACTER_H
#define SRC_VIC3WORLD_CHARACTERS_CHARACTER_H



#include <istream>



namespace vic3
{

class Character
{
  public:
   explicit Character(int id,
       std::string first_name,
       std::string last_name,
       int culture_id,
       std::set<std::string> roles,
       int rank,
       std::string ideology,
       std::set<std::string> traits):
       id_(id),
       first_name_(std::move(first_name)),
       last_name_(std::move(last_name)),
       culture_id_(culture_id),
       roles_(std::move(roles)),
       rank_(rank),
       ideology_(std::move(ideology)),
       traits_(std::move(traits))
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
};
}  // namespace vic3



#endif  // SRC_VIC3WORLD_CHARACTERS_CHARACTER_H