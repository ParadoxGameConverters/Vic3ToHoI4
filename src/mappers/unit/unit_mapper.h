#ifndef SRC_MAPPERS_UNIT_UNITMAPPER_H
#define SRC_MAPPERS_UNIT_UNITMAPPER_H

#include <map>
#include <set>
#include <string>
#include <vector>

#include "src/hoi4_world/military/battalion.h"
#include "src/vic3_world/military/military_formation.h"



namespace mappers
{

using BattalionMap = std::map<std::string, float>;


struct BattalionTemplate
{
   int equipment;
   BattalionMap units;
};


using TemplateMap = std::map<std::string, BattalionTemplate>;

class UnitMapper
{
  public:
   explicit UnitMapper(TemplateMap& templates): templates_(std::move(templates)) {}

   [[nodiscard]] std::vector<hoi4::Battalion> MakeBattalions(const std::vector<std::string>& methods, int scale) const;
   [[nodiscard]] std::vector<hoi4::Battalion> MakeBattalions(const vic3::MilitaryFormation& formation) const;

  private:
   TemplateMap templates_;
   mutable std::set<std::string> warned_;
};

}  // namespace mappers



#endif  // SRC_MAPPERS_UNIT_UNITMAPPER_H