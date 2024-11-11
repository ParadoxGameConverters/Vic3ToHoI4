#ifndef SRC_HOI4WORLD_CHARACTERS_HOI4CHARACTER_H
#define SRC_HOI4WORLD_CHARACTERS_HOI4CHARACTER_H



#include <istream>
#include <optional>
#include <set>



namespace hoi4
{
struct Admiral
{
   std::set<std::string> traits;
   int attack = 1;
   int defense = 1;
   int maneuvering = 1;
   int coordination = 1;

   std::partial_ordering operator<=>(const Admiral&) const = default;
};
struct General
{
   std::set<std::string> traits;
   bool is_field_marshal = false;
   int attack = 1;
   int defense = 1;
   int planning = 1;
   int logistics = 1;

   std::partial_ordering operator<=>(const General&) const = default;
};
struct Leader
{
   std::string sub_ideology;
   std::set<std::string> traits;

   std::partial_ordering operator<=>(const Leader&) const = default;
};
struct Advisor
{
   std::set<std::string> traits;
   std::string slot;
   std::string ledger;
   int cost = 150;

   std::partial_ordering operator<=>(const Advisor&) const = default;
};
struct Spy
{
   std::set<std::string> traits;
   std::set<std::string> nationalities;

   std::partial_ordering operator<=>(const Spy&) const = default;
};

struct CharacterOptions
{
   int id = 0;
   std::string first_name;
   std::string last_name;
   std::string portrait_alias;
   bool is_female = false;
   std::optional<Admiral> admiral_data;
   std::optional<General> general_data;
   std::optional<Leader> leader_data;
   std::optional<Advisor> advisor_data;
   std::optional<Spy> spy_data;
};
class Character
{
  public:
   explicit Character(CharacterOptions options):
       id_(options.id),
       first_name_(std::move(options.first_name)),
       last_name_(std::move(options.last_name)),
       portrait_alias_(std::move(options.portrait_alias)),
       is_female_(options.is_female),
       admiral_data_(std::move(options.admiral_data)),
       general_data_(std::move(options.general_data)),
       leader_data_(std::move(options.leader_data)),
       advisor_data_(std::move(options.advisor_data)),
       spy_data_(std::move(options.spy_data))
   {
   }

   [[nodiscard]] int GetId() const { return id_; }
   [[nodiscard]] const std::string& GetFirstName() const { return first_name_; }
   [[nodiscard]] const std::string& GetLastName() const { return last_name_; }

   [[nodiscard]] const std::string& GetPortraitAlias() const { return portrait_alias_; }
   [[nodiscard]] bool IsFemale() const { return is_female_; }
   [[nodiscard]] const std::optional<Admiral>& GetAdmiralData() const { return admiral_data_; }
   [[nodiscard]] const std::optional<General>& GetGeneralData() const { return general_data_; }
   [[nodiscard]] const std::optional<Leader>& GetLeaderData() const { return leader_data_; }
   [[nodiscard]] const std::optional<Advisor>& GetAdvisorData() const { return advisor_data_; }
   [[nodiscard]] const std::optional<Spy>& GetSpyData() const { return spy_data_; }

   void SetPortraitAlias(const std::string& alias) { portrait_alias_ = alias; }

   [[nodiscard]] static int GetGenId() { return gen_id_; }
   static void IncrementGenId() { ++gen_id_; }

   std::partial_ordering operator<=>(const Character&) const = default;

  private:
   int id_ = 0;
   std::string first_name_;
   std::string last_name_;

   std::string portrait_alias_;
   bool is_female_ = false;
   std::optional<Admiral> admiral_data_;
   std::optional<General> general_data_;
   std::optional<Leader> leader_data_;
   std::optional<Advisor> advisor_data_;
   std::optional<Spy> spy_data_;

   inline static int gen_id_ = 1000;  // For generating new characters, like councils
};
}  // namespace hoi4



#endif  // SRC_HOI4WORLD_CHARACTERS_HOI4CHARACTER_H