#include "src/maps/map_data_importer.h"

#include <external/fmt/include/fmt/format.h>

#include <external/bitmap/bitmap_image.hpp>

#include "src/support/progress_manager.h"


namespace
{

commonItems::Color GetCenterColor(const maps::Point position, const bitmap_image& province_map)
{
   rgb_t color{0, 0, 0};
   province_map.get_pixel(position.x, position.y, color);

   return commonItems::Color(std::array<int, 3>{color.red, color.green, color.blue});
}


commonItems::Color GetAboveColor(maps::Point position, const bitmap_image& province_map)
{
   if (position.y > 0)
   {
      position.y--;
   }

   rgb_t color{0, 0, 0};
   province_map.get_pixel(position.x, position.y, color);

   return commonItems::Color(std::array<int, 3>{color.red, color.green, color.blue});
}


commonItems::Color GetBelowColor(maps::Point position, int height, const bitmap_image& province_map)
{
   if (position.y < height - 1)
   {
      position.y++;
   }

   rgb_t color{0, 0, 0};
   province_map.get_pixel(position.x, position.y, color);

   return commonItems::Color(std::array<int, 3>{color.red, color.green, color.blue});
}


commonItems::Color GetLeftColor(maps::Point position, int width, const bitmap_image& province_map)
{
   if (position.x > 0)
   {
      position.x--;
   }
   else
   {
      position.x = width - 1;
   }

   rgb_t color{0, 0, 0};
   province_map.get_pixel(position.x, position.y, color);

   return commonItems::Color(std::array<int, 3>{color.red, color.green, color.blue});
}


commonItems::Color GetRightColor(maps::Point position, int width, const bitmap_image& province_map)
{
   if (position.x < width - 1)
   {
      position.x++;
   }
   else
   {
      position.x = 0;
   }

   rgb_t color{0, 0, 0};
   province_map.get_pixel(position.x, position.y, color);

   return commonItems::Color(std::array<int, 3>{color.red, color.green, color.blue});
}

}  // namespace



maps::MapData maps::MapDataImporter::ImportMapData(const commonItems::ModFilesystem& mod_filesystem)
{
   Log(LogLevel::Info) << "    -> Importing Provinces";
   ImportProvinces(mod_filesystem);
   Log(LogLevel::Info) << "    -> Importing Adjacencies";
   ImportAdjacencies(mod_filesystem);

   return maps::MapData({.province_neighbors = province_neighbors_,
       .borders = borders_,
       .the_province_points = the_province_points_,
       .province_definitions = province_definitions_,
       .points_to_provinces = points_to_provinces_});
}


void maps::MapDataImporter::ImportProvinces(const commonItems::ModFilesystem& mod_filesystem)
{
   const auto path = mod_filesystem.GetActualFileLocation("map/provinces.bmp");
   if (!path)
   {
      throw std::runtime_error("Could not find map/provinces.bmp");
   }

   Log(LogLevel::Info) << fmt::format("      -> opening {}", path->string());
   bitmap_image province_map(path->string());
   if (!province_map)
   {
      throw std::runtime_error(fmt::format("Could not open {}/map/provinces.bmp", path->string()));
   }

   Log(LogLevel::Info) << "      -> scanning colors";
   int prev_progress = 0;
   const int height = static_cast<int>(province_map.height());
   const int width = static_cast<int>(province_map.width());
   for (int y = 0; y < height; y++)
   {
      for (int x = 0; x < width; x++)
      {
         Point position = {x, y};

         auto center_color = GetCenterColor(position, province_map);
         auto above_color = GetAboveColor(position, province_map);
         auto below_color = GetBelowColor(position, height, province_map);
         auto left_color = GetLeftColor(position, width, province_map);
         auto right_color = GetRightColor(position, width, province_map);

         position.y = height - y - 1;
         if (center_color != above_color)
         {
            HandleNeighbor(center_color, above_color, position);
         }
         if (center_color != right_color)
         {
            HandleNeighbor(center_color, right_color, position);
         }
         if (center_color != below_color)
         {
            HandleNeighbor(center_color, below_color, position);
         }
         if (center_color != left_color)
         {
            HandleNeighbor(center_color, left_color, position);
         }

         if (auto province = province_definitions_.GetProvinceFromColor(center_color); province)
         {
            points_to_provinces_.emplace(position, *province);
            if (auto specific_province_points = the_province_points_.find(*province);
                specific_province_points != the_province_points_.end())
            {
               specific_province_points->second.AddPoint(position);
            }
            else
            {
               ProvincePoints the_new_points;
               the_new_points.AddPoint(position);
               the_province_points_.emplace(*province, the_new_points);
            }
         }
      }
      int current_progress = (10 * y / height);
      if (prev_progress != current_progress)
      {
         ProgressManager::AddProgress(1);
         prev_progress = current_progress;
      }
   }
}


void maps::MapDataImporter::HandleNeighbor(const commonItems::Color& center_color,
    const commonItems::Color& other_color,
    const Point& position)
{
   const auto center_province = province_definitions_.GetProvinceFromColor(center_color);
   const auto other_province = province_definitions_.GetProvinceFromColor(other_color);
   if (center_province && other_province)
   {
      AddNeighbor(*center_province, *other_province);
      AddPointToBorder(*center_province, *other_province, position);
   }
}


void maps::MapDataImporter::AddNeighbor(const std::string& main_province, const std::string& neighbor_province)
{
   if (const auto center_mapping = province_neighbors_.find(main_province); center_mapping != province_neighbors_.end())
   {
      center_mapping->second.insert(neighbor_province);
   }
   else
   {
      const std::set<std::string> neighbors = {neighbor_province};
      province_neighbors_[main_province] = neighbors;
   }
}


void maps::MapDataImporter::RemoveNeighbor(const std::string& main_province, const std::string& neighbor_province)
{
   if (const auto center_mapping = province_neighbors_.find(main_province); center_mapping != province_neighbors_.end())
   {
      center_mapping->second.erase(neighbor_province);
   }
}


void maps::MapDataImporter::AddPointToBorder(const std::string& main_province,
    const std::string& neighbor_province,
    const Point position)
{
   auto borders_with_neighbors = borders_.find(main_province);
   if (borders_with_neighbors == borders_.end())
   {
      BordersWith new_borders_with_neighbors;
      borders_.emplace(main_province, new_borders_with_neighbors);
      borders_with_neighbors = borders_.find(main_province);
   }

   auto border = borders_with_neighbors->second.find(neighbor_province);
   if (border == borders_with_neighbors->second.end())
   {
      BorderPoints new_border;
      borders_with_neighbors->second.emplace(neighbor_province, new_border);
      border = borders_with_neighbors->second.find(neighbor_province);
   }

   if (border->second.empty())
   {
      border->second.push_back(position);
   }
   else
   {
      if (const auto last_point = border->second.back(); (last_point.x != position.x) || (last_point.y != position.y))
      {
         border->second.push_back(position);
      }
   }
}


void maps::MapDataImporter::ImportAdjacencies(const commonItems::ModFilesystem& mod_filesystem)
{
   const auto path = mod_filesystem.GetActualFileLocation("map/adjacencies.csv");
   if (!path)
   {
      throw std::runtime_error("Could not find map/adjacencies.csv");
   }

   Log(LogLevel::Info) << fmt::format("      -> opening {}", path->string());
   std::ifstream adjacencies_file(*path);
   if (!adjacencies_file.is_open())
   {
      throw std::runtime_error(fmt::format("Could not open {}", path->string()));
   }

   Log(LogLevel::Info) << "      -> scanning file";
   while (!adjacencies_file.eof())
   {
      std::string line;
      getline(adjacencies_file, line);
      if (line.starts_with('#'))
      {
         continue;
      }

      const std::regex pattern("([^;]*);([^;]*);([^;]*)(.*)\r?");
      if (std::smatch matches; regex_match(line, matches, pattern))
      {
         if (matches[1] == "From" || matches[1] == "-1")
         {
            continue;
         }

         if (matches[1].length() == 0)
         {
            continue;
         }
         const std::string first_province = matches[1];
         if (matches[2].length() == 0)
         {
            continue;
         }
         const std::string second_province = matches[2];
         if (matches[3] != "impassable")
         {
            AddNeighbor(first_province, second_province);
            AddNeighbor(second_province, first_province);
         }
         else
         {
            RemoveNeighbor(first_province, second_province);
            RemoveNeighbor(second_province, first_province);
         }
      }
   }
}