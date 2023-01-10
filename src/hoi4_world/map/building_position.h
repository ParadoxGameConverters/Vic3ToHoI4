#ifndef SRC_HOI4WORLD_MAP_BUILDINGPOSITION_H
#define SRC_HOI4WORLD_MAP_BUILDINGPOSITION_H



namespace hoi4
{

struct BuildingPosition
{
   double x_coordinate = 0.0;
   double y_coordinate = 0.0;
   double z_coordinate = 0.0;
   double rotation = 0.0;

   bool operator==(const BuildingPosition&) const = default;
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_MAP_BUILDINGPOSITION_H