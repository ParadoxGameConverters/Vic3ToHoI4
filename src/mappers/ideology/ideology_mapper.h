#ifndef SRC_MAPPERS_IDEOLOGY_IDEOLOGYMAPPER_H
#define SRC_MAPPERS_IDEOLOGY_IDEOLOGYMAPPER_H



#include <string>



namespace mappers
{

class IdeologyMapper
{
  public:
   IdeologyMapper() = default;

   std::string GetRulingIdeology(const std::set<std::string>& current_laws) const;
};

}  // namespace mappers



#endif  // SRC_MAPPERS_IDEOLOGY_IDEOLOGYMAPPER_H