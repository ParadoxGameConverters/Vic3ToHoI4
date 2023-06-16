#include "src/mappers/culture/culture_graphics_mapper.h"

#include "external/fmt/include/fmt/format.h"

namespace
{
std::vector<std::string> operator+(const std::vector<std::string>& lhs, const std::vector<std::string>& rhs)
{
   std::set<std::string> unique_paths;
   unique_paths.insert(lhs.cbegin(), lhs.cend());
   unique_paths.insert(rhs.cbegin(), rhs.cend());

   std::vector<std::string> joined;
   joined.insert(joined.end(), unique_paths.begin(), unique_paths.end());
   return joined;
}

mappers::IdeologyPortraitPaths operator+(const mappers::IdeologyPortraitPaths& lhs,
    const mappers::IdeologyPortraitPaths& rhs)
{
   return {lhs.communism + rhs.communism,
       lhs.democratic + rhs.democratic,
       lhs.fascism + rhs.fascism,
       lhs.neutrality + rhs.neutrality};
}

mappers::PortraitPaths operator+(const mappers::PortraitPaths& lhs, const mappers::PortraitPaths& rhs)
{
   return {lhs.army + rhs.army,
       lhs.navy + rhs.navy,
       lhs.leader + rhs.leader,
       lhs.female_leader + rhs.female_leader,
       lhs.ideology_minister + rhs.ideology_minister,
       lhs.male_operative + rhs.male_operative,
       lhs.female_operative + rhs.female_operative,
       lhs.male_monarch + rhs.male_monarch,
       lhs.female_monarch + rhs.female_monarch};
}

mappers::GraphicsBlock operator+(const mappers::GraphicsBlock& lhs, const mappers::GraphicsBlock& rhs)
{
   // Union two GraphicsBlocks, this is for multi-ethnic cultures
   if (lhs.graphical_culture.empty())
   {
      return {{lhs.portrait_paths + rhs.portrait_paths}, rhs.graphical_culture, rhs.graphical_culture_2d};
   }
   return {{lhs.portrait_paths + rhs.portrait_paths}, lhs.graphical_culture, lhs.graphical_culture_2d};
}
}  // namespace

mappers::GraphicsBlock mappers::CultureGraphicsMapper::MatchPrimaryCulturesToGraphics(
    const std::set<std::string>& primary_cultures,
    const std::map<std::string, vic3::CultureDefinition>& cultures) const
{
   GraphicsBlock graphics_block;
   for (const auto& culture: primary_cultures)
   {
      if (const auto culture_itr = cultures.find(culture); culture_itr != cultures.end())
      {
         graphics_block = graphics_block + MatchCultureToGraphics(culture_itr->second);
      }
      else
      {
         Log(LogLevel::Warning) << fmt::format("Culture: {} has no definition.", culture);
      }
   }
   return graphics_block;
}

mappers::GraphicsBlock mappers::CultureGraphicsMapper::MatchCultureToGraphics(
    const vic3::CultureDefinition& culture_def) const
{
   GraphicsBlock graphics_block;

   // Match culture, there may be more than one match
   bool matched = false;
   for (const auto& mapping: mappings_)
   {
      if (mapping.cultures.contains(culture_def.GetName()))
      {
         matched = true;
         graphics_block = graphics_block + mapping.graphics_block;
      }
   }
   if (matched)
   {
      return graphics_block;
   }

   // Fallback to match traits, there may be more than one match
   for (const auto& mapping: mappings_)
   {
      for (const auto& trait: culture_def.GetTraits())
      {
         if (mapping.traits.contains(trait))
         {
            matched = true;
            graphics_block = graphics_block + mapping.graphics_block;
         }
      }
   }
   if (matched)
   {
      return graphics_block;
   }

   // Last fallback to match ethnicities, there may be more than one match
   for (const auto& mapping: mappings_)
   {
      for (const auto& ethnicity: culture_def.GetEthnicities())
      {
         if (mapping.ethnicities.contains(ethnicity))
         {
            matched = true;
            graphics_block = graphics_block + mapping.graphics_block;
         }
      }
   }
   if (!matched)
   {
      Log(LogLevel::Warning) << fmt::format("Culture {} has no matching portrait set.", culture_def.GetName());
   }
   // If matched graphics block is not empty, return, otherwise error msg
   return graphics_block;
}
