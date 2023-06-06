#ifndef SRC_MAPPERS_CULTURE_CULTUREGRAPHICSMAPPING_H
#define SRC_MAPPERS_CULTURE_CULTUREGRAPHICSMAPPING_H



#include <string>


namespace mappers
{
struct IdeologyPortraitPaths
{
   std::set<std::string> communism;
   std::set<std::string> democratic;
   std::set<std::string> fascism;
   std::set<std::string> neutrality;

   std::partial_ordering operator<=>(const IdeologyPortraitPaths&) const = default;
};

struct PortraitPaths
{
   std::set<std::string> army;
   std::set<std::string> navy;
   IdeologyPortraitPaths leader;
   std::set<std::string> female_leader;
   IdeologyPortraitPaths ideology_minister;
   std::set<std::string> male_operative;
   std::set<std::string> female_operative;
   std::set<std::string> male_monarch;
   std::set<std::string> female_monarch;

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