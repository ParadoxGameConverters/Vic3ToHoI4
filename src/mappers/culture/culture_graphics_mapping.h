#ifndef SRC_MAPPERS_CULTURE_CULTUREGRAPHICSMAPPING_H
#define SRC_MAPPERS_CULTURE_CULTUREGRAPHICSMAPPING_H



#include <map>
#include <set>
#include <string>
#include <vector>



namespace mappers
{
using PortraitName = std::string;
using PortraitType = std::string;
using PortraitPaths = std::map<PortraitType, std::vector<PortraitName>>;


struct GraphicsBlock
{
   PortraitPaths portrait_paths;

   std::string graphical_culture;
   std::string graphical_culture_2d;

   std::partial_ordering operator<=>(const GraphicsBlock&) const = default;
};


struct CultureGraphicsMapping
{
   std::set<std::string> cultures;
   std::set<std::string> traits;
   std::set<std::string> ethnicities;

   GraphicsBlock graphics_block;

   std::partial_ordering operator<=>(const CultureGraphicsMapping&) const = default;
};

}  // namespace mappers



#endif  // SRC_MAPPERS_CULTURE_CULTUREGRAPHICSMAPPING_H