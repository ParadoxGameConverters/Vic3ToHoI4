#ifndef SRC_HOI4WORLD_CHARACTERS_HOI4CHARACTER_H
#define SRC_HOI4WORLD_CHARACTERS_HOI4CHARACTER_H



#include <istream>



namespace hoi4
{
struct Admiral
{
   std::set<std::string> traits;
   int skill = 1;
   int attack = 1;
   int defense = 1;
   int maneuvering = 1;
   int coordination = 1;

   std::partial_ordering operator<=>(const Admiral&) const = default;
};
struct General
{
   std::set<std::string> traits;
   int skill = 1;
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

   std::partial_ordering operator<=>(const Advisor&) const = default;
};
struct Spy
{
   std::set<std::string> traits;
   std::set<std::string> nationalities;
   int skill = 1;

   std::partial_ordering operator<=>(const Spy&) const = default;
};

struct CharacterOptions
{
   int id = 0;
   std::string first_name;
   std::string last_name;
   std::string portrait_alias;
   Admiral admiral_data;
   General general_data;
   Leader leader_data;
   Advisor advisor_data;
   Spy spy_data;
};
class Character
{
  public:
   explicit Character(CharacterOptions options):
       id_(options.id),
       first_name_(std::move(options.first_name)),
       last_name_(std::move(options.last_name)),
       portrait_alias_(std::move(options.last_name)),
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
   [[nodiscard]] const Admiral& GetAdmiralData() const { return admiral_data_; }
   [[nodiscard]] const General& GetGeneralData() const { return general_data_; }
   [[nodiscard]] const Leader& GetLeaderData() const { return leader_data_; }
   [[nodiscard]] const Advisor& GetAdvisorData() const { return advisor_data_; }
   [[nodiscard]] const Spy& GetSpyData() const { return spy_data_; }

   std::partial_ordering operator<=>(const Character&) const = default;

  private:
   int id_ = 0;
   std::string first_name_;
   std::string last_name_;

   std::string portrait_alias_;
   Admiral admiral_data_;
   General general_data_;
   Leader leader_data_;
   Advisor advisor_data_;
   Spy spy_data_;
};
}  // namespace hoi4



#endif  // SRC_HOI4WORLD_CHARACTERS_HOI4CHARACTER_H