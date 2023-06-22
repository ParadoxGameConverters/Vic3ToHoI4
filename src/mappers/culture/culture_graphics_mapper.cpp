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
       lhs.advisor + rhs.advisor,
       lhs.male_operative + rhs.male_operative,
       lhs.female_operative + rhs.female_operative,
       lhs.male_monarch + rhs.male_monarch,
       lhs.female_monarch + rhs.female_monarch,
       lhs.council + rhs.council};
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

std::vector<std::string>& ValueOr(std::vector<std::string>& lhs,
    std::vector<std::string>& mhs,
    std::vector<std::string>& rhs)
{
   if (!lhs.empty())
   {
      return lhs;
   }
   if (!mhs.empty())
   {
      return mhs;
   }
   return rhs;
}

mappers::IdeologyPortraitPaths IdeologyValueOr(mappers::IdeologyPortraitPaths& lhs,
    mappers::IdeologyPortraitPaths& mhs,
    mappers::IdeologyPortraitPaths& rhs)
{
   return {
       ValueOr(lhs.communism, mhs.communism, rhs.communism),
       ValueOr(lhs.democratic, mhs.democratic, rhs.democratic),
       ValueOr(lhs.fascism, mhs.fascism, rhs.fascism),
       ValueOr(lhs.neutrality, mhs.neutrality, rhs.neutrality),
   };
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
      return GraphicsBlock({{}, "western_european", "western_european_2d"});
   }
   if (!(unit_graphics.contains("graphical_culture") && unit_graphics.contains("graphical_culture_2d")))
   {
      Log(LogLevel::Warning) << fmt::format("Culture {} lacks unit graphics. Defaulting to western.",
          culture_def.GetName());
      unit_graphics.emplace("graphical_culture", "western_european");
      unit_graphics.emplace("graphical_culture_2d", "western_european_2d");
   }

   // Fill in lower blocks from higher blocks.
   return GraphicsBlock(
       {
           ValueOr(culture_paths.army, trait_paths.army, ethnicity_paths.army),
           ValueOr(culture_paths.navy, trait_paths.navy, ethnicity_paths.navy),
           IdeologyValueOr(culture_paths.leader, trait_paths.leader, ethnicity_paths.leader),
           ValueOr(culture_paths.female_leader, trait_paths.female_leader, ethnicity_paths.female_leader),
           IdeologyValueOr(culture_paths.advisor, trait_paths.advisor, ethnicity_paths.advisor),
           ValueOr(culture_paths.male_operative, trait_paths.male_operative, ethnicity_paths.male_operative),
           ValueOr(culture_paths.female_operative, trait_paths.female_operative, ethnicity_paths.female_operative),
           ValueOr(culture_paths.male_monarch, trait_paths.male_monarch, ethnicity_paths.male_monarch),
           ValueOr(culture_paths.female_monarch, trait_paths.female_monarch, ethnicity_paths.female_monarch),
           ValueOr(culture_paths.council, trait_paths.council, ethnicity_paths.council),
       },
       unit_graphics.at("graphical_culture"),
       unit_graphics.at("graphical_culture_2d"));
}
