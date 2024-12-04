#ifndef SRC_HOI4WORLD_MAP_BUILDING_H
#define SRC_HOI4WORLD_MAP_BUILDING_H



#include <optional>
#include <string>

#include "src/hoi4_world/map/building_position.h"



namespace hoi4
{

struct BuildingOptions
{
   int state_id = 0;
   std::string type;
   BuildingPosition position;
   std::optional<int> connecting_sea_province;
};


class Building
{
  public:
   Building(BuildingOptions options):
       state_id_(options.state_id),
       type_(std::move(options.type)),
       position_(std::move(options.position)),
       connecting_sea_province_(std::move(options.connecting_sea_province))
   {
   }

   bool operator==(const Building&) const = default;

   [[nodiscard]] int GetStateId() const { return state_id_; }
   [[nodiscard]] std::string GetType() const { return type_; }
   [[nodiscard]] BuildingPosition GetPosition() const { return position_; }
   [[nodiscard]] std::optional<int> GetConnectingSeaProvince() const { return connecting_sea_province_; }

   // This allows the Google test framework to print human-readable countries if a test fails.
   friend void PrintTo(const Building& building, std::ostream* os);

  private:
   int state_id_ = 0;
   std::string type_;
   BuildingPosition position_;
   std::optional<int> connecting_sea_province_;
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_MAP_BUILDING_H
