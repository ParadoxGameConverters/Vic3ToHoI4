#ifndef SRC_HOI4WORLD_WORLD_HOI4WORLD_H
#define SRC_HOI4WORLD_WORLD_HOI4WORLD_H



#include <external/commonItems/Localization/LocalizationDatabase.h>

#include <map>
#include <memory>
#include <string>

#include "src/hoi4_world/characters/hoi4_character.h"
#include "src/hoi4_world/countries/hoi4_country.h"
#include "src/hoi4_world/decisions/decision.h"
#include "src/hoi4_world/decisions/decisions_category.h"
#include "src/hoi4_world/localizations/localizations.h"
#include "src/hoi4_world/map/buildings.h"
#include "src/hoi4_world/map/railways.h"
#include "src/hoi4_world/map/strategic_regions.h"
#include "src/hoi4_world/states/hoi4_states.h"



namespace hoi4
{

struct WorldOptions
{
   std::map<std::string, Country> countries;
   std::set<std::string> great_powers;
   std::set<std::string> major_powers;
   States states;
   StrategicRegions strategic_regions;
   Buildings buildings;
   Railways railways;
   Localizations localizations;
   std::map<int, Character> characters;
};


class World
{
  public:
   explicit World(WorldOptions options):
       countries_(std::move(options.countries)),
       great_powers_(std::move(options.great_powers)),
       major_powers_(std::move(options.major_powers)),
       states_(std::move(options.states)),
       strategic_regions_(std::move(options.strategic_regions)),
       buildings_(std::move(options.buildings)),
       railways_(std::move(options.railways)),
       localizations_(std::move(options.localizations)),
       characters_(std::move(options.characters))
   {
   }

   [[nodiscard]] const std::map<std::string, Country>& GetCountries() const { return countries_; }
   [[nodiscard]] const std::set<std::string>& GetGreatPowers() const { return great_powers_; }
   [[nodiscard]] const std::set<std::string>& GetMajorPowers() const { return major_powers_; }
   [[nodiscard]] const States& GetStates() const { return states_; }
   [[nodiscard]] const StrategicRegions& GetStrategicRegions() const { return strategic_regions_; }
   [[nodiscard]] const Buildings& GetBuildings() const { return buildings_; }
   [[nodiscard]] const Railways& GetRailways() const { return railways_; }
   [[nodiscard]] const Localizations& GetLocalizations() const { return localizations_; }
   [[nodiscard]] const std::map<int, Character>& GetCharacters() const { return characters_; }
   [[nodiscard]] const std::set<DecisionsCategory>& GetDecisionsCategories() const { return decisions_categories_; }
   [[nodiscard]] const std::map<std::string, std::vector<Decision>>& GetDecisionsInCategories() const
   {
      return decisions_in_categories_;
   }

   [[nodiscard]] std::map<std::string, Country>& GetModifiableCountries() { return countries_; }

   void SetDecisionsCategories(std::set<DecisionsCategory> decisions_categories)
   {
      decisions_categories_ = std::move(decisions_categories);
   }
   void SetDecisions(std::map<std::string, std::vector<Decision>> decisions_in_categories)
   {
      decisions_in_categories_ = std::move(decisions_in_categories);
   }

   void SetCountryEvents(std::map<std::string, std::vector<Event>> country_events_)
   {
      country_events_ = std::move(country_events_);
   }

  private:
   std::map<std::string, Country> countries_;
   std::set<std::string> great_powers_;
   std::set<std::string> major_powers_;
   States states_;
   StrategicRegions strategic_regions_;
   Buildings buildings_;
   Railways railways_;
   Localizations localizations_;
   std::map<int, Character> characters_;

   std::set<DecisionsCategory> decisions_categories_;
   std::map<std::string, std::vector<Decision>> decisions_in_categories_;

   std::map<std::string, std::vector<Event>> country_events_;
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_WORLD_HOI4WORLD_H