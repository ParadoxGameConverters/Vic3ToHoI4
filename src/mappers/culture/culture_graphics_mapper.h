#ifndef SRC_MAPPERS_CULTURE_CULTUREGRAPHICSMAPPER_H
#define SRC_MAPPERS_CULTURE_CULTUREGRAPHICSMAPPER_H



#include <vector>

#include "src/mappers/culture/culture_graphics_mapping.h"
#include "src/vic3_world/cultures/culture_definition.h"



namespace mappers
{

class CultureGraphicsMapper
{
  public:
   explicit CultureGraphicsMapper(std::vector<CultureGraphicsMapping> mappings): mappings_(std::move(mappings)) {}

   [[nodiscard]] GraphicsBlock MatchPrimaryCulturesToGraphics(const std::set<std::string>& primary_cultures,
       const std::map<std::string, vic3::CultureDefinition>& cultures) const;
   [[nodiscard]] GraphicsBlock MatchCultureToGraphics(const vic3::CultureDefinition& culture_def) const;

   void CheckMappings(const std::map<std::string, vic3::CultureDefinition>& source_cultures) const;

  private:
   std::vector<CultureGraphicsMapping> mappings_;
};

}  // namespace mappers



#endif  // SRC_MAPPERS_CULTURE_CULTUREGRAPHICSMAPPER_H