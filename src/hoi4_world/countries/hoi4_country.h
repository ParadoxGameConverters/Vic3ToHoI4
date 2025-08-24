#ifndef SRC_HOI4WORLD_COUNTRIES_HOI4COUNTRY_H
#define SRC_HOI4WORLD_COUNTRIES_HOI4COUNTRY_H



#include <external/commonItems/Color.h>
#include <external/commonItems/Date.h>

#include <string>

#include "src/hoi4_world/diplomacy/hoi4_war.h"
#include "src/hoi4_world/focus_trees/focus_tree.h"
#include "src/hoi4_world/military/equipment_variant.h"
#include "src/hoi4_world/military/ship.h"
#include "src/hoi4_world/military/task_force.h"
#include "src/hoi4_world/technology/technologies.h"
#include "src/mappers/country/country_mapper.h"
#include "src/mappers/culture/culture_graphics_mapping.h"



namespace hoi4
{

struct NameList
{
   std::set<std::string> male_names;
   std::set<std::string> female_names;
   std::set<std::string> surnames;
   std::set<std::string> male_surnames;
   std::set<std::string> female_surnames;

   std::partial_ordering operator<=>(const NameList&) const = default;
};

struct Unit
{
   std::string unit_template;
   int equipment;
   int location;

   std::partial_ordering operator<=>(const Unit&) const = default;
};

struct CountryOptions
{
   int source_country_number;
   std::string tag;
   commonItems::Color color;
   std::set<int> owned_states;
   std::optional<int> capital_state;
   std::set<std::string> primary_cultures;
   std::string ideology = "neutrality";
   std::string sub_ideology = "despotism";
   std::map<std::string, int> ideology_support;
   date last_election{1933, 1, 1};
   bool has_elections;
   Technologies technologies;
   std::vector<EquipmentVariant> legacy_ship_variants;
   std::vector<EquipmentVariant> ship_variants;
   std::vector<EquipmentVariant> plane_variants;
   std::vector<EquipmentVariant> tank_variants;
   std::set<std::string> ideas;
   std::string economy_law = "civilian_economy";
   std::string trade_law = "export_focus";
   std::string military_law = "volunteer_only";
   mappers::GraphicsBlock graphics_block;
   NameList name_list;
   std::vector<int> character_ids;
   std::set<int> spy_ids;
   std::optional<int64_t> monarch_idea_id;
   std::set<std::string> puppets;
   std::optional<std::string> overlord;
   int starting_research_slots = 3;
   std::vector<Unit> units;
   float stability = 0.0F;
   int convoys = 0;
   std::vector<TaskForce> task_forces;
   std::vector<War> wars;
};

class Country
{
  public:
   explicit Country(CountryOptions country_options):
       source_country_number_(country_options.source_country_number),
       tag_(std::move(country_options.tag)),
       color_(country_options.color),
       owned_states_(std::move(country_options.owned_states)),
       capital_state_(country_options.capital_state),
       primary_cultures_(std::move(country_options.primary_cultures)),
       ideology_(std::move(country_options.ideology)),
       sub_ideology_(std::move(country_options.sub_ideology)),
       ideology_support_(std::move(country_options.ideology_support)),
       last_election_(country_options.last_election),
       has_elections_(country_options.has_elections),
       technologies_(std::move(country_options.technologies)),
       legacy_ship_variants_(std::move(country_options.legacy_ship_variants)),
       ship_variants_(std::move(country_options.ship_variants)),
       plane_variants_(std::move(country_options.plane_variants)),
       tank_variants_(std::move(country_options.tank_variants)),
       ideas_(std::move(country_options.ideas)),
       economy_law_(std::move(country_options.economy_law)),
       trade_law_(std::move(country_options.trade_law)),
       military_law_(std::move(country_options.military_law)),
       graphics_block_(std::move(country_options.graphics_block)),
       name_list_(std::move(country_options.name_list)),
       character_ids_(std::move(country_options.character_ids)),
       spy_ids_(std::move(country_options.spy_ids)),
       monarch_idea_id_(country_options.monarch_idea_id),
       puppets_(std::move(country_options.puppets)),
       overlord_(std::move(country_options.overlord)),
       starting_research_slots_(country_options.starting_research_slots),
       stability_(country_options.stability),
       units_(country_options.units),
       convoys_(country_options.convoys),
       task_forces_(country_options.task_forces),
       wars_(country_options.wars)
   {
   }

   [[nodiscard]] int GetSourceCountryNumber() const { return source_country_number_; }
   [[nodiscard]] const std::string& GetTag() const { return tag_; }
   [[nodiscard]] const commonItems::Color& GetColor() const { return color_; }
   [[nodiscard]] const std::set<int>& GetOwnedStates() const { return owned_states_; }
   [[nodiscard]] const std::optional<int>& GetCapitalState() const { return capital_state_; }
   [[nodiscard]] const std::set<std::string>& GetPrimaryCultures() const { return primary_cultures_; }
   [[nodiscard]] int GetConvoys() const { return convoys_; }
   [[nodiscard]] const std::string& GetIdeology() const { return ideology_; }
   [[nodiscard]] const std::string& GetSubIdeology() const { return sub_ideology_; }
   [[nodiscard]] const std::map<std::string, int>& GetIdeologySupport() const { return ideology_support_; }
   [[nodiscard]] const date& GetLastElection() const { return last_election_; }
   [[nodiscard]] bool AreElectionsAllowed() const { return has_elections_; }
   [[nodiscard]] const Technologies& GetTechnologies() const { return technologies_; }
   [[nodiscard]] const std::vector<EquipmentVariant>& GetLegacyShipVariants() const { return legacy_ship_variants_; }
   [[nodiscard]] const std::vector<EquipmentVariant>& GetShipVariants() const { return ship_variants_; }
   [[nodiscard]] const std::vector<EquipmentVariant>& GetPlaneVariants() const { return plane_variants_; }
   [[nodiscard]] const std::vector<EquipmentVariant>& GetTankVariants() const { return tank_variants_; }
   [[nodiscard]] const std::set<std::string>& GetIdeas() const { return ideas_; }
   [[nodiscard]] std::vector<std::string> GetLaws() const { return {economy_law_, trade_law_, military_law_}; }
   [[nodiscard]] const mappers::GraphicsBlock& GetGraphicsBlock() const { return graphics_block_; }
   [[nodiscard]] const NameList& GetNameList() const { return name_list_; }
   [[nodiscard]] const std::vector<int>& GetLeaderIds() const { return character_ids_; }
   [[nodiscard]] const std::set<int>& GetSpyIds() const { return spy_ids_; }
   [[nodiscard]] const std::optional<int64_t>& GetMonarchIdeaIds() const { return monarch_idea_id_; }
   [[nodiscard]] const std::set<std::string>& GetPuppets() const { return puppets_; }
   [[nodiscard]] const std::optional<std::string>& GetOverlord() const { return overlord_; }
   [[nodiscard]] int GetStartingResearchSlots() const { return starting_research_slots_; }
   [[nodiscard]] float GetStability() const { return stability_; }
   [[nodiscard]] float GetWarSupport() const { return 0.60F; }
   [[nodiscard]] const std::vector<TaskForce>& GetTaskForces() const { return task_forces_; }
   [[nodiscard]] const std::vector<Unit>& GetUnits() const { return units_; }
   [[nodiscard]] const std::vector<War>& GetWars() const { return wars_; }
   [[nodiscard]] const FocusTree& GetFocusTree() const { return focus_tree_; }

   void AddWar(War war) { wars_.emplace_back(std::move(war)); }
   void RemovePuppets(const std::set<std::string>& puppet_tags);
   void SetFocusTree(const FocusTree& focus_tree) { focus_tree_ = focus_tree; }

   std::partial_ordering operator<=>(const Country&) const = default;

   // This allows the Google test framework to print human-readable countries if a test fails.
   friend void PrintTo(const Country& country, std::ostream* os);

  private:
   // if making changes here also update PrintTo()
   int source_country_number_;
   std::string tag_;
   commonItems::Color color_;
   std::set<int> owned_states_;
   std::optional<int> capital_state_;
   std::set<std::string> primary_cultures_;
   std::string ideology_ = "neutrality";
   std::string sub_ideology_ = "despotism";
   std::map<std::string, int> ideology_support_{{"neutrality", 100}};
   date last_election_{1933, 1, 1};
   bool has_elections_ = false;
   Technologies technologies_;
   std::vector<EquipmentVariant> legacy_ship_variants_;
   std::vector<EquipmentVariant> ship_variants_;
   std::vector<EquipmentVariant> plane_variants_;
   std::vector<EquipmentVariant> tank_variants_;
   std::set<std::string> ideas_;
   std::string economy_law_ = "civilian_economy";
   std::string trade_law_ = "export_focus";
   std::string military_law_ = "volunteer_only";
   mappers::GraphicsBlock graphics_block_;
   NameList name_list_;
   std::vector<int> character_ids_;
   std::set<int> spy_ids_;
   std::optional<int64_t> monarch_idea_id_;
   std::set<std::string> puppets_;
   std::optional<std::string> overlord_;
   int starting_research_slots_;
   float stability_ = 0.0F;
   std::vector<Unit> units_;
   int convoys_;
   std::vector<TaskForce> task_forces_;
   std::vector<War> wars_;
   FocusTree focus_tree_;
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_COUNTRIES_HOI4COUNTRY_H
