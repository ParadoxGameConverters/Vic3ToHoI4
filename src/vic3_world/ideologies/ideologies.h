#ifndef SRC_VIC3WORLD_IDEOLOGIES_IDEOLOGIES_H
#define SRC_VIC3WORLD_IDEOLOGIES_IDEOLOGIES_H



#include <map>
#include <string>

#include "src/vic3_world/ideologies/ideology.h"



namespace vic3
{


class Ideologies
{
  public:
   Ideologies() = default;
   explicit Ideologies(std::map<std::string, Ideology> ideologies): ideologies_(std::move(ideologies)) {}

   [[nodiscard]] std::map<Law, ApprovalAmount> CalculateLawApprovals(const std::set<std::string>& ideologies) const;

  private:
   std::map<std::string, Ideology> ideologies_;
};


}  // namespace vic3



#endif  // SRC_VIC3WORLD_IDEOLOGIES_IDEOLOGIES_H