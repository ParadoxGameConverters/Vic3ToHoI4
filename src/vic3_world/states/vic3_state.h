#ifndef SRC_VIC3WORLD_STATES_VIC3STATE_H
#define SRC_VIC3WORLD_STATES_VIC3STATE_H



#include <ranges>
#include <set>

#include "src/vic3_world/buildings/building.h"

namespace vic3
{
class World;

struct StateOptions
{
   int id;
   std::optional<int> owner_number;
   std::optional<std::string> owner_tag;
   bool incorporated = false;
   float infrastructure = 0.0F;
   std::set<int> provinces;
   int population = 0;
   int employed_population = 0;
};


class State
{
  public:
   State() = default;
   explicit State(StateOptions state_options):
       id_(state_options.id),
       owner_number_(state_options.owner_number),
       owner_tag_(std::move(state_options.owner_tag)),
       incorporated_(state_options.incorporated),
       infrastructure_(state_options.infrastructure),
       provinces_(std::move(state_options.provinces)),
       population_(state_options.population),
       employed_population_(state_options.employed_population)
   {
   }

   [[nodiscard]] int GetId() const { return id_; }
   [[nodiscard]] const std::optional<int>& GetOwnerNumber() const { return owner_number_; }
   [[nodiscard]] const std::optional<std::string>& GetOwnerTag() const { return owner_tag_; }
   [[nodiscard]] bool IsIncorporated() const { return incorporated_; }
   [[nodiscard]] float GetInfrastructure() const { return infrastructure_; }
   [[nodiscard]] const std::set<int>& GetProvinces() const { return provinces_; }
   [[nodiscard]] int GetPopulation() const { return population_; }
   [[nodiscard]] int GetEmployedPopulation() const { return employed_population_; }
   [[nodiscard]] std::vector<Building> GetBuildings(const World& source_world);

   void SetOwnerTag(std::string_view owner_tag) { owner_tag_ = owner_tag; }

   bool operator==(const State&) const = default;

  private:
   int id_;
   std::optional<int> owner_number_;
   std::optional<std::string> owner_tag_;
   bool incorporated_ = false;
   float infrastructure_ = 0.0F;
   std::set<int> provinces_;
   int population_ = 0;
   int employed_population_ = 0;
};

}  // namespace vic3



#endif  // SRC_VIC3WORLD_STATES_VIC3STATE_H