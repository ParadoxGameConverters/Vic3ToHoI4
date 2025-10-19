#include "src/mappers/culture/culture_graphics_mapper.h"

#include <external/commonItems/Log.h>
#include <external/fmt/include/fmt/format.h>

#include <algorithm>
#include <ranges>



namespace
{

mappers::PortraitPaths operator+(const mappers::PortraitPaths& lhs, const mappers::PortraitPaths& rhs)
{
   mappers::PortraitPaths sum;
   for (const auto& side: {lhs, rhs})
   {
      for (const auto& [key, portraits]: side)
      {
         if (auto [itr, success] = sum.emplace(key, portraits); !success)
         {
            std::set<std::string> current_portraits;
            std::ranges::copy(itr->second, std::inserter(current_portraits, current_portraits.begin()));

            std::ranges::copy_if(portraits,
                std::back_inserter(itr->second),
                [current_portraits](const std::string& portrait) {
                   return !current_portraits.contains(portrait);
                });
         }
      }
   }
   return sum;
}


mappers::GraphicsBlock operator+(const mappers::GraphicsBlock& lhs, const mappers::GraphicsBlock& rhs)
{
   // Union two GraphicsBlocks, this is for multi-ethnic cultures
   if (lhs.graphical_culture.empty())
   {
      return {
          .portrait_paths = {lhs.portrait_paths + rhs.portrait_paths},
          .graphical_culture = rhs.graphical_culture,
          .graphical_culture_2d = rhs.graphical_culture_2d,
      };
   }
   return {
       .portrait_paths = {lhs.portrait_paths + rhs.portrait_paths},
       .graphical_culture = lhs.graphical_culture,
       .graphical_culture_2d = lhs.graphical_culture_2d,
   };
}


mappers::PortraitPaths ValueOr(mappers::PortraitPaths& lhs, mappers::PortraitPaths& mhs, mappers::PortraitPaths& rhs)
{
   mappers::PortraitPaths paths;
   for (const auto& side: {lhs, mhs, rhs})
   {
      for (const auto& [key, portraits]: side)
      {
         paths.emplace(key, portraits);
      }
   }
   return paths;
}


void EmplaceUnitGraphics(std::map<std::string, std::string>& unit_graphics, const mappers::GraphicsBlock& block)
{
   if (!block.graphical_culture.empty())
   {
      unit_graphics.emplace("graphical_culture", block.graphical_culture);
   }
   if (!block.graphical_culture_2d.empty())
   {
      unit_graphics.emplace("graphical_culture_2d", block.graphical_culture_2d);
   }
}

}  // namespace


namespace mappers
{

GraphicsBlock mappers::CultureGraphicsMapper::MatchPrimaryCulturesToGraphics(
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


GraphicsBlock CultureGraphicsMapper::MatchCultureToGraphics(const vic3::CultureDefinition& culture_def) const
{
   // Match culture, there may be more than one match
   bool matched = false;
   std::map<std::string, std::string> unit_graphics;  // First come, first serve
   PortraitPaths culture_paths;
   for (const auto& mapping: mappings_)
   {
      if (mapping.cultures.contains(culture_def.GetName()))
      {
         matched = true;
         culture_paths = culture_paths + mapping.graphics_block.portrait_paths;
         EmplaceUnitGraphics(unit_graphics, mapping.graphics_block);
      }
   }

   // Fallback to match traits, there may be more than one match
   PortraitPaths trait_paths;
   for (const auto& mapping: mappings_)
   {
      for (const auto& trait: culture_def.GetTraits())
      {
         if (mapping.traits.contains(trait))
         {
            matched = true;
            trait_paths = trait_paths + mapping.graphics_block.portrait_paths;
            EmplaceUnitGraphics(unit_graphics, mapping.graphics_block);
         }
      }
   }

   // Last fallback to match ethnicities, there may be more than one match
   PortraitPaths ethnicity_paths;
   for (const auto& mapping: mappings_)
   {
      for (const auto& ethnicity: culture_def.GetEthnicities())
      {
         if (mapping.ethnicities.contains(ethnicity))
         {
            matched = true;
            ethnicity_paths = ethnicity_paths + mapping.graphics_block.portrait_paths;
            EmplaceUnitGraphics(unit_graphics, mapping.graphics_block);
         }
      }
   }

   // If matched graphics block is not empty, return, otherwise error msg
   if (!matched)
   {
      Log(LogLevel::Warning) << fmt::format("Culture {} has no matching portrait set.", culture_def.GetName());
      return GraphicsBlock({
          .graphical_culture = "western_european_gfx",
          .graphical_culture_2d = "western_european_2d",
      });
   }
   if (!(unit_graphics.contains("graphical_culture") && unit_graphics.contains("graphical_culture_2d")))
   {
      Log(LogLevel::Warning) << fmt::format("Culture {} lacks unit graphics. Defaulting to western.",
          culture_def.GetName());
      unit_graphics.emplace("graphical_culture", "western_european_gfx");
      unit_graphics.emplace("graphical_culture_2d", "western_european_2d");
   }

   // Fill in lower blocks from higher blocks.
   return {
       .portrait_paths = ValueOr(culture_paths, trait_paths, ethnicity_paths),
       .graphical_culture = unit_graphics.at("graphical_culture"),
       .graphical_culture_2d = unit_graphics.at("graphical_culture_2d"),
   };
}


void CultureGraphicsMapper::CheckMappings(const std::map<std::string, vic3::CultureDefinition>& source_cultures) const
{
   for (const auto& culture: source_cultures | std::ranges::views::values)
   {
      bool matched = false;
      for (const auto& mapping: mappings_)
      {
         if (mapping.cultures.contains(culture.GetName()))
         {
            matched = true;
            break;
         }

         for (const std::string& trait: culture.GetTraits())
         {
            if (mapping.traits.contains(trait))
            {
               matched = true;
               break;
            }
         }
         if (matched)
         {
            break;
         }
         for (const std::string& ethnicity: culture.GetEthnicities())
         {
            if (mapping.ethnicities.contains(ethnicity))
            {
               matched = true;
               break;
            }
         }
         if (matched)
         {
            break;
         }
      }

      if (!matched)
      {
         Log(LogLevel::Warning) << fmt::format("Culture: {} has no mapping.", culture.GetName());
      }
   }
}

}  // namespace mappers