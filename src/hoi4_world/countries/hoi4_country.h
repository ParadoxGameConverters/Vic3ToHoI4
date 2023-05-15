#ifndef SRC_HOI4WORLD_COUNTRIES_HOI4COUNTRY_H
#define SRC_HOI4WORLD_COUNTRIES_HOI4COUNTRY_H



#include <string>

#include "external/commonItems/Color.h"
#include "src/hoi4_world/military/equipment_variant.h"
#include "src/hoi4_world/technology/technologies.h"
#include "src/mappers/country/country_mapper.h"



namespace hoi4
{

struct CountryOptions
{
   std::string tag;
   commonItems::Color color;
   std::optional<int> capital_state;
   std::string ideology = "neutrality";
   std::string sub_ideology = "despotism";
   Technologies technologies;
   std::vector<EquipmentVariant> legacy_ship_variants;
   std::vector<EquipmentVariant> ship_variants;
   std::vector<EquipmentVariant> plane_variants;
   std::vector<EquipmentVariant> tank_variants;
   std::set<std::string> ideas;
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
       technologies_(std::move(country_options.technologies)),
       legacy_ship_variants_(std::move(country_options.legacy_ship_variants)),
       ship_variants_(std::move(country_options.ship_variants)),
       plane_variants_(std::move(country_options.plane_variants)),
       tank_variants_(std::move(country_options.tank_variants)),
       ideas_(std::move(country_options.ideas))
   {
   }

   [[nodiscard]] bool AreElectionsAllowed() const;

   [[nodiscard]] const std::string& GetTag() const { return tag_; }
   [[nodiscard]] const commonItems::Color& GetColor() const { return color_; }
   [[nodiscard]] const std::optional<int>& GetCapitalState() const { return capital_state_; }
   [[nodiscard]] const std::string& GetIdeology() const { return ideology_; }
   [[nodiscard]] const std::string& GetSubIdeology() const { return sub_ideology_; }
   [[nodiscard]] const std::map<std::string, int>& GetIdeologySupport() const { return ideology_support_; }
   [[nodiscard]] const date& GetLastElection() const { return last_election_; }
   [[nodiscard]] const Technologies& GetTechnologies() const { return technologies_; }
   [[nodiscard]] const std::vector<EquipmentVariant>& GetLegacyShipVariants() const { return legacy_ship_variants_; }
   [[nodiscard]] const std::vector<EquipmentVariant>& GetShipVariants() const { return ship_variants_; }
   [[nodiscard]] const std::vector<EquipmentVariant>& GetPlaneVariants() const { return plane_variants_; }
   [[nodiscard]] const std::vector<EquipmentVariant>& GetTankVariants() const { return tank_variants_; }
   [[nodiscard]] const std::set<std::string>& GetIdeas() const { return ideas_; }

   std::partial_ordering operator<=>(const Country&) const = default;

  private:
   std::string tag_;
   commonItems::Color color_;
   std::optional<int> capital_state_;
   std::string ideology_ = "neutrality";
   std::string sub_ideology_ = "despotism";
   std::map<std::string, int> ideology_support_{{"neutrality", 100}};
   date last_election_{1933, 1, 1};
   Technologies technologies_;
   std::vector<EquipmentVariant> legacy_ship_variants_;
   std::vector<EquipmentVariant> ship_variants_;
   std::vector<EquipmentVariant> plane_variants_;
   std::vector<EquipmentVariant> tank_variants_;
   std::set<std::string> ideas_;
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_COUNTRIES_HOI4COUNTRY_H