#ifndef SRC_MAPPERS_UNIT_UNITMAPPER_H
#define SRC_MAPPERS_UNIT_UNITMAPPER_H

#include <map>
#include <set>
#include <string>

#include "src/hoi4_world/military/battalion.h"

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

   std::vector<hoi4::Battalion> MakeBattalions(const std::vector<std::string>& methods, int scale) const;

  private:
   TemplateMap templates_;
   static std::set<std::string> warned_;
};

}  // namespace mappers


#endif  // SRC_MAPPERS_UNIT_UNITMAPPER_H
