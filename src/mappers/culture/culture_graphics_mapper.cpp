#include "src/mappers/culture/culture_graphics_mapper.h"

namespace
{
mappers::GraphicsBlock operator+(const mappers::GraphicsBlock& lhs, const mappers::GraphicsBlock& rhs)
{
   // Union two GraphicsBlocks
   // Combine all portrait sets, this is for multi-ethnic cultures
   return {{}, lhs.graphical_culture, lhs.graphical_culture_2d};
}
}  // namespace

mappers::GraphicsBlock mappers::CultureGraphicsMapper::MatchCultureToGraphics(
    const vic3::CultureDefinition& culture_def) const
{
   // Match culture, there should only be one
   for (const auto& mapping: mappings_)
   {
      if (mapping.cultures.contains(culture_def.GetName()))
      {
         return mapping.graphics_block;
      }
   }
   // Match traits, there may be more than one match
   for (const auto& mapping: mappings_)
   {
      for (const auto& trait: culture_def.GetTraits())
      {
         // If there is match add to graphics
      }
   }
   // If matched graphics block is not empty, return

   // Match ethnicities, there may be more than one match
   // If matched graphics block is not empty, return, otherwise error msg
   return GraphicsBlock();
}
