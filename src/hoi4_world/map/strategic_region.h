#ifndef SRC_HOI4WORLD_MAP_STRATEGICREGION_H
#define SRC_HOI4WORLD_MAP_STRATEGICREGION_H



#include <map>
#include <optional>
#include <string>
#include <vector>



namespace hoi4
{

class StrategicRegion
{
  public:
   explicit StrategicRegion(std::string filename,
       int id,
       std::string name,
       std::vector<int> old_provinces,
       std::map<std::string, std::string> static_modifiers,
       std::optional<std::string> naval_terrain,
       std::string weather):
       filename_(std::move(filename)),
       id_(id),
       name_(std::move(name)),
       old_provinces_(std::move(old_provinces)),
       static_modifiers_(std::move(static_modifiers)),
       naval_terrain_(std::move(naval_terrain)),
       weather_(std::move(weather))
   {
   }

   [[nodiscard]] const auto& GetFilename() const { return filename_; }
   [[nodiscard]] auto GetID() const { return id_; }
   [[nodiscard]] std::string_view GetName() const { return name_; }
   [[nodiscard]] const auto& GetOldProvinces() const { return old_provinces_; }
   [[nodiscard]] const auto& GetNewProvinces() const { return new_provinces_; }
   [[nodiscard]] bool hasStaticModifiers() const { return !static_modifiers_.empty(); }
   [[nodiscard]] const auto& GetStaticModifiers() const { return static_modifiers_; }
   [[nodiscard]] const auto& GetNavalTerrain() const { return naval_terrain_; }
   [[nodiscard]] std::string_view GetWeather() const { return weather_; }

   void AddNewProvince(const int province) { new_provinces_.push_back(province); }

   std::strong_ordering operator<=>(const StrategicRegion&) const = default;

  private:
   std::string filename_;
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
