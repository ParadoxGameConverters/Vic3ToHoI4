#ifndef SRC_MAPPERS_CULTURE_CULTUREGRAPHICSMAPPING_H
#define SRC_MAPPERS_CULTURE_CULTUREGRAPHICSMAPPING_H



#include <string>


namespace mappers
{
struct IdeologyPortraitPaths
{
   std::vector<std::string> communism;
   std::vector<std::string> democratic;
   std::vector<std::string> fascism;
   std::vector<std::string> neutrality;

   std::partial_ordering operator<=>(const IdeologyPortraitPaths&) const = default;
};

struct PortraitPaths
{
   std::vector<std::string> army;
   std::vector<std::string> navy;
   IdeologyPortraitPaths leader;
   std::vector<std::string> female_leader;
   IdeologyPortraitPaths ideology_minister;
   std::vector<std::string> male_operative;
   std::vector<std::string> female_operative;
   std::vector<std::string> male_monarch;
   std::vector<std::string> female_monarch;

   std::partial_ordering operator<=>(const PortraitPaths&) const = default;
};

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