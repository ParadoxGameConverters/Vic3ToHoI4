#ifndef SRC_MAPS_PROVINCEPOINTS_H
#define SRC_MAPS_PROVINCEPOINTS_H



#include <climits>
#include <set>

#include "src/maps/point.h"



namespace maps
{

class ProvincePoints
{
  public:
   ProvincePoints() = default;
   ProvincePoints(std::set<Point> points);

   void AddPoint(const Point& the_point);

   [[nodiscard]] Point GetCentermostPoint() const;

  private:
   std::set<Point> points_;
   Point leftmost_point_ = {.x = INT_MAX, .y = 0};
   Point rightmost_point_ = {.x = 0, .y = 0};
   Point highest_point_ = {.x = 0, .y = 0};
   Point lowest_point_ = {.x = 0, .y = INT_MAX};
};

}  // namespace maps



#endif  // SRC_MAPS_PROVINCEPOINTS_H