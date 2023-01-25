#include "src/maps/province_points.h"

#include <numeric>



namespace
{

double CalculateDistanceSquared(const maps::Point point, const maps::Point center)
{
   const auto delta_x = static_cast<double>(point.x) - static_cast<double>(center.x);
   const auto delta_y = static_cast<double>(point.y) - static_cast<double>(center.y);
   return delta_x * delta_x + delta_y * delta_y;
}

}  // namespace



maps::ProvincePoints::ProvincePoints(std::set<Point> points)
{
   points_ = points;
   for (const auto& point: points)
   {
      if (point.x < leftmost_point_.x)
      {
         leftmost_point_ = point;
      }
      if (point.x > rightmost_point_.x)
      {
         rightmost_point_ = point;
      }
      if (point.y < lowest_point_.y)
      {
         lowest_point_ = point;
      }
      if (point.y > highest_point_.y)
      {
         highest_point_ = point;
      }
   }
}



void maps::ProvincePoints::AddPoint(const Point& the_point)
{
   points_.emplace(the_point);
   if (the_point.x < leftmost_point_.x)
   {
      leftmost_point_ = the_point;
   }
   if (the_point.x > rightmost_point_.x)
   {
      rightmost_point_ = the_point;
   }
   if (the_point.y < lowest_point_.y)
   {
      lowest_point_ = the_point;
   }
   if (the_point.y > highest_point_.y)
   {
      highest_point_ = the_point;
   }
}


maps::Point maps::ProvincePoints::GetCentermostPoint() const
{
   Point possible_center;
   possible_center.x = std::midpoint(leftmost_point_.x, rightmost_point_.x);
   possible_center.y = std::midpoint(lowest_point_.y, highest_point_.y);
   if (points_.contains(possible_center))
   {
      return possible_center;
   }

   auto shortest_distance = std::numeric_limits<double>::max();
   Point closest_point{0, 0};
   for (const auto& possible_point: points_)
   {
      const auto distance_squared = CalculateDistanceSquared(possible_point, possible_center);
      if (distance_squared < shortest_distance)
      {
         shortest_distance = distance_squared;
         closest_point = possible_point;
      }
   }
   return closest_point;
}