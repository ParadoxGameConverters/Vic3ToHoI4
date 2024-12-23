#include "src/maps/map_data.h"

#include <external/commonItems/Color.h>
#include <external/commonItems/Log.h>
#include <external/fmt/include/fmt/format.h>

#include <external/bitmap/bitmap_image.hpp>
#include <fstream>



std::set<std::string> maps::MapData::GetNeighbors(const std::string& province) const
{
   const auto neighbors = province_neighbors_.find(province);
   if (neighbors == province_neighbors_.end())
   {
      return {};
   }

   return neighbors->second;
}


std::optional<maps::Point> maps::MapData::GetSpecifiedBorderCenter(const std::string& main_province,
    const std::string& neighbor) const
{
   const auto borders_with_neighbors = borders_.find(main_province);
   if (borders_with_neighbors == borders_.end())
   {
      Log(LogLevel::Warning) << fmt::format("Province {} has no borders.", main_province);
      return std::nullopt;
   }

   const auto border = borders_with_neighbors->second.find(neighbor);
   if (border == borders_with_neighbors->second.end())
   {
      Log(LogLevel::Warning) << fmt::format("Province {} does not border {}.", main_province, neighbor);
      return std::nullopt;
   }

   return border->second[(border->second.size() / 2)];
}


std::optional<maps::Point> maps::MapData::GetAnyBorderCenter(const std::string& province) const
{
   const auto borders_with_neighbors = borders_.find(province);
   if (borders_with_neighbors == borders_.end())
   {
      Log(LogLevel::Warning) << fmt::format("Province {} has no borders.", province);
      return std::nullopt;
   }

   const auto border = borders_with_neighbors->second.begin();
   // if a province has borders, by definition they're with some number of neighbors and of some length
   return border->second[(border->second.size() / 2)];
}


std::optional<maps::Point> maps::MapData::GetCentermostPoint(const std::string& province) const
{
   const auto possible_points = the_province_points_.find(province);
   if (possible_points == the_province_points_.end())
   {
      return std::nullopt;
   }
   return possible_points->second.GetCentermostPoint();
}


std::optional<std::string> maps::MapData::GetProvinceName(const Point& point) const
{
   const auto i = points_to_provinces_.find(point);
   if (i == points_to_provinces_.end())
   {
      return std::nullopt;
   }
   return i->second;
}


std::optional<maps::ProvincePoints> maps::MapData::GetProvincePoints(const std::string& province_num) const
{
   const auto possible_points = the_province_points_.find(province_num);
   if (possible_points == the_province_points_.end())
   {
      return std::nullopt;
   }
   return possible_points->second;
}