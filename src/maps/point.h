#ifndef SRC_MAPS_POINT_H
#define SRC_MAPS_POINT_H



#include <compare>



namespace maps
{

struct Point
{
   int x = 0;
   int y = 0;

   std::strong_ordering operator<=>(const Point&) const = default;
};

}  // namespace maps



#endif  // SRC_MAPS_POINT_H