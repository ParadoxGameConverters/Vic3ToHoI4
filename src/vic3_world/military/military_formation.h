#ifndef SRC_VIC3WORLD_MILITARY_MILITARYFORMATION_H
#define SRC_VIC3WORLD_MILITARY_MILITARYFORMATION_H



namespace vic3
{

enum class MilitaryFormationType
{
   kArmy,
   kFleet
};


struct MilitaryFormation
{
   int country;
   MilitaryFormationType type;
   std::optional<std::string> name;
   std::optional<int> ordinal_number;
   std::map<std::string, int> units;

   std::strong_ordering operator<=>(const MilitaryFormation&) const = default;
};

}  // namespace vic3



#endif  // SRC_VIC3WORLD_MILITARY_MILITARYFORMATION_H