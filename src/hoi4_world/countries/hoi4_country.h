#ifndef SRC_HOI4WORLD_COUNTRIES_HOI4COUNTRY_H
#define SRC_HOI4WORLD_COUNTRIES_HOI4COUNTRY_H



#include <string>

#include "external/commonItems/Color.h"
#include "external/commonItems/Date.h"
#include "src/hoi4_world/military/equipment_variant.h"
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

struct CountryOptions
{
   std::string tag;
   commonItems::Color color;
   std::optional<int> capital_state;
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
   mappers::GraphicsBlock graphics_block;
   NameList name_list;
   std::vector<int> character_ids;
   std::set<int> spy_ids;
   std::set<std::string> puppets;
};


class Country
{
  public:
   explicit Country(CountryOptions country_options):
       tag_(std::move(country_options.tag)),
       color_(country_options.color),
       capital_state_(country_options.capital_state),
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
       graphics_block_(std::move(country_options.graphics_block)),
       name_list_(std::move(country_options.name_list)),
       character_ids_(std::move(country_options.character_ids)),
       spy_ids_(std::move(country_options.spy_ids)),
       puppets_(std::move(country_options.puppets))
   {
   }


   [[nodiscard]] const std::string& GetTag() const { return tag_; }
   [[nodiscard]] const commonItems::Color& GetColor() const { return color_; }
   [[nodiscard]] const std::optional<int>& GetCapitalState() const { return capital_state_; }
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
   [[nodiscard]] const mappers::GraphicsBlock& GetGraphicsBlock() const { return graphics_block_; }
   [[nodiscard]] const NameList& GetNameList() const { return name_list_; }
   [[nodiscard]] const std::vector<int>& GetLeaderIds() const { return character_ids_; }
   [[nodiscard]] const std::set<int>& GetSpyIds() const { return spy_ids_; }
   [[nodiscard]] const std::set<std::string>& GetPuppets() const { return puppets_; }

   std::partial_ordering operator<=>(const Country&) const = default;

  private:
   std::string tag_;
   commonItems::Color color_;
   std::optional<int> capital_state_;
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
   mappers::GraphicsBlock graphics_block_;
   NameList name_list_;
   std::vector<int> character_ids_;
   std::set<int> spy_ids_;
   std::set<std::string> puppets_;
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_COUNTRIES_HOI4COUNTRY_H