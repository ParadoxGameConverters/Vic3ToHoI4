#ifndef SRC_HOI4WORLD_STATES_HOI4STATE_H
#define SRC_HOI4WORLD_STATES_HOI4STATE_H



#include <compare>
#include <optional>
#include <set>
#include <string>

#include "src/hoi4_world/map/resources_map.h"



namespace hoi4
{

struct StateOptions
{
   std::optional<std::string> owner;
   std::set<int> provinces;
   int manpower = 0;
   Resources resources;
   std::string category = "rural";

   std::map<int, int> victory_points;

   int civilian_factories = 0;
   int military_factories = 0;
   int dockyards = 0;
   std::optional<int> naval_base_location;
   std::optional<int> naval_base_level;
   std::set<std::string> cores;
};


class State
{
  public:
   State(int id, StateOptions state_options):
       id_(id),
       owner_(std::move(state_options.owner)),
       provinces_(std::move(state_options.provinces)),
       manpower_(state_options.manpower),
       resources_(std::move(state_options.resources)),
       category_(std::move(state_options.category)),
       victory_points_(std::move(state_options.victory_points)),
       civilian_factories_(state_options.civilian_factories),
       military_factories_(state_options.military_factories),
       dockyards_(state_options.dockyards),
       naval_base_location_(state_options.naval_base_location),
       naval_base_level_(state_options.naval_base_level),
       cores_(state_options.cores)
   {
   }

   [[nodiscard]] int GetId() const { return id_; }
   [[nodiscard]] const std::optional<std::string>& GetOwner() const { return owner_; }
   [[nodiscard]] const std::set<int>& GetProvinces() const { return provinces_; }
   [[nodiscard]] int GetManpower() const { return manpower_; }
   [[nodiscard]] const Resources& GetResources() const { return resources_; }
   [[nodiscard]] const std::string& GetCategory() const { return category_; }
   [[nodiscard]] const std::map<int, int>& GetVictoryPoints() const { return victory_points_; }
   [[nodiscard]] int GetCivilianFactories() const { return civilian_factories_; }
   [[nodiscard]] int GetMilitaryFactories() const { return military_factories_; }
   [[nodiscard]] int GetDockyards() const { return dockyards_; }
   [[nodiscard]] std::optional<int> GetNavalBaseLocation() const { return naval_base_location_; }
   [[nodiscard]] std::optional<int> GetNavalBaseLevel() const { return naval_base_level_; }
   [[nodiscard]] std::set<std::string> GetCores() const { return cores_; }

   std::partial_ordering operator<=>(const State&) const = default;

  private:
   int id_;
   std::optional<std::string> owner_;
   std::set<int> provinces_;
   int manpower_ = 0;
   Resources resources_;
   std::string category_ = "rural";

   std::map<int, int> victory_points_;

   int civilian_factories_ = 0;
   int military_factories_ = 0;
   int dockyards_ = 0;
   std::optional<int> naval_base_location_;
   std::optional<int> naval_base_level_;
   std::set<std::string> cores_;
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_STATES_HOI4STATE_H