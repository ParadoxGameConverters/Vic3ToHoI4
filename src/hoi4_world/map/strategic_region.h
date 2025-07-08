#ifndef SRC_HOI4WORLD_MAP_STRATEGICREGION_H
#define SRC_HOI4WORLD_MAP_STRATEGICREGION_H



#include <compare>
#include <filesystem>
#include <map>
#include <optional>
#include <string>
#include <vector>



namespace hoi4
{

struct StrategicRegionOptions
{
   std::filesystem::path filename;
   int id = 0;
   std::string name;
   std::vector<int> old_provinces;
   std::map<std::string, std::string> static_modifiers;
   std::optional<std::string> naval_terrain;
   std::string weather;
};


class StrategicRegion
{
  public:
   explicit StrategicRegion(StrategicRegionOptions options):
       filename_(std::move(options.filename)),
       id_(options.id),
       name_(std::move(options.name)),
       old_provinces_(std::move(options.old_provinces)),
       static_modifiers_(std::move(options.static_modifiers)),
       naval_terrain_(std::move(options.naval_terrain)),
       weather_(std::move(options.weather))
   {
   }

   [[nodiscard]] const std::filesystem::path& GetFilename() const { return filename_; }
   [[nodiscard]] int GetID() const { return id_; }
   [[nodiscard]] std::string_view GetName() const { return name_; }
   [[nodiscard]] const std::vector<int>& GetOldProvinces() const { return old_provinces_; }
   [[nodiscard]] const std::vector<int>& GetNewProvinces() const { return new_provinces_; }
   [[nodiscard]] bool HasStaticModifiers() const { return !static_modifiers_.empty(); }
   [[nodiscard]] const std::map<std::string, std::string>& GetStaticModifiers() const { return static_modifiers_; }
   [[nodiscard]] const std::optional<std::string>& GetNavalTerrain() const { return naval_terrain_; }
   [[nodiscard]] std::string_view GetWeather() const { return weather_; }

   void AddNewProvince(const int province) { new_provinces_.push_back(province); }

   std::strong_ordering operator<=>(const StrategicRegion&) const = default;

  private:
   std::filesystem::path filename_;
   int id_ = 0;
   std::string name_;
   std::vector<int> old_provinces_;
   std::vector<int> new_provinces_;
   std::map<std::string, std::string> static_modifiers_;
   std::optional<std::string> naval_terrain_;
   std::string weather_;
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_MAP_STRATEGICREGION_H
