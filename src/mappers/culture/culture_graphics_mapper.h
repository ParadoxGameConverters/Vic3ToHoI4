#ifndef SRC_MAPPERS_CULTURE_CULTUREGRAPHICSMAPPER_H
#define SRC_MAPPERS_CULTURE_CULTUREGRAPHICSMAPPER_H



#include "src/mappers/culture/culture_graphics_mapping.h"
#include "src/vic3_world/cultures/culture_definition.h"


namespace mappers
{

class CultureGraphicsMapper
{
  public:
   explicit CultureGraphicsMapper(std::vector<CultureGraphicsMapping> mappings): mappings_(std::move(mappings)) {}

   GraphicsBlock MatchPrimaryCulturesToGraphics(const std::set<std::string>& primary_cultures,
       const std::map<std::string, vic3::CultureDefinition>& cultures) const;
   GraphicsBlock MatchCultureToGraphics(const vic3::CultureDefinition& culture_def) const;

  private:
   std::vector<CultureGraphicsMapping> mappings_;
};

}  // namespace mappers



#endif  // SRC_MAPPERS_CULTURE_CULTUREGRAPHICSMAPPER_H