#include "character_trait_mapper.h"

#include <climits>

#include "external/fmt/include/fmt/format.h"

namespace
{
std::map<hoi4::Trait, int> used_traits;
}

hoi4::Admiral mappers::CharacterTraitMapper::GetAdmiralMappedData(const std::set<vic3::Trait>& source_traits) const
{
   hoi4::Admiral admiral_mapping{
       .attack = 1,
       .defense = 1,
       .maneuvering = 1,
       .coordination = 1,
   };
   for (const vic3::Trait& vic3_trait: source_traits)
   {
      if (const auto& mapping_itr = admiral_trait_rules_.find(vic3_trait); mapping_itr != admiral_trait_rules_.end())
      {
         const std::set<hoi4::Trait>& hoi4_traits = mapping_itr->second.traits;
         admiral_mapping.traits.insert(hoi4_traits.begin(), hoi4_traits.end());
         admiral_mapping.attack += mapping_itr->second.attack;
         admiral_mapping.defense += mapping_itr->second.defense;
         admiral_mapping.maneuvering += mapping_itr->second.maneuvering;
         admiral_mapping.coordination += mapping_itr->second.coordination;
      }
   }

   // HoI4 clamps skill to 1
   admiral_mapping.attack = std::max(1, admiral_mapping.attack);
   admiral_mapping.defense = std::max(1, admiral_mapping.defense);
   admiral_mapping.maneuvering = std::max(1, admiral_mapping.maneuvering);
   admiral_mapping.coordination = std::max(1, admiral_mapping.coordination);

   return admiral_mapping;
}

hoi4::General mappers::CharacterTraitMapper::GetGeneralMappedData(const std::set<vic3::Trait>& source_traits,
    const bool is_field_marshal) const
{
   hoi4::General general_mapping{
       .is_field_marshal = is_field_marshal,
       .attack = 1,
       .defense = 1,
       .planning = 1,
       .logistics = 1,
   };
   for (const vic3::Trait& vic3_trait: source_traits)
   {
      if (const auto& mapping_itr = general_trait_rules_.find(vic3_trait); mapping_itr != general_trait_rules_.end())
      {
         const std::set<hoi4::Trait>& general_traits = mapping_itr->second.traits;
         general_mapping.traits.insert(general_traits.begin(), general_traits.end());
         if (is_field_marshal)
         {
            const std::set<hoi4::Trait>& marshal_traits = mapping_itr->second.field_marshal_traits;
            general_mapping.traits.insert(marshal_traits.begin(), marshal_traits.end());
         }
         general_mapping.attack += mapping_itr->second.attack;
         general_mapping.defense += mapping_itr->second.defense;
         general_mapping.planning += mapping_itr->second.planning;
         general_mapping.logistics += mapping_itr->second.logistics;
      }
   }

   // HoI4 clamps skill to 1
   general_mapping.attack = std::max(1, general_mapping.attack);
   general_mapping.defense = std::max(1, general_mapping.defense);
   general_mapping.planning = std::max(1, general_mapping.planning);
   general_mapping.logistics = std::max(1, general_mapping.logistics);

   return general_mapping;
}

std::set<hoi4::Trait> mappers::CharacterTraitMapper::GetSpyMappedTraits(
    const std::set<vic3::Trait>& source_traits) const
{
   std::set<hoi4::Trait> hoi4_traits;
   for (const vic3::Trait& vic3_trait: source_traits)
   {
      if (const auto& trait_itr = spy_trait_rules_.find(vic3_trait); trait_itr != spy_trait_rules_.end())
      {
         hoi4_traits.emplace(trait_itr->second);
      }
   }
   return hoi4_traits;
}

std::set<hoi4::Trait> mappers::CharacterTraitMapper::GetAdvisorMappedTraits(
    const std::set<vic3::Trait>& source_traits) const
{
   // Count Vic3 traits for each HoI4 trait.
   std::map<hoi4::Trait, int> trait_points;
   for (const vic3::Trait& vic3_trait: source_traits)
   {
      if (!advisor_trait_rules_.contains(vic3_trait))
      {
         continue;
      }
      trait_points[advisor_trait_rules_.at(vic3_trait)]++;
   }
   std::set<hoi4::Trait> hoi4_traits;
   if (trait_points.empty())
   {
      return hoi4_traits;
   }

   // Reduce to highest-scoring candidate traits.
   int most_points = 0;
   for (const auto [hoi4_trait, points]: trait_points)
   {
      if (points < most_points)
      {
         continue;
      }
      if (points > most_points)
      {
         hoi4_traits.clear();
         most_points = points;
      }
      hoi4_traits.emplace(hoi4_trait);
   }

   if (hoi4_traits.size() < 2)
   {
      return hoi4_traits;
   }

   // Pick least-used trait among the remainder.
   hoi4::Trait rarest;
   int least_used = INT_MAX;
   for (const auto& cand: hoi4_traits)
   {
      auto curr_use = used_traits[cand];
      if (curr_use >= least_used)
      {
         continue;
      }
      least_used = curr_use;
      rarest = cand;
   }

   hoi4_traits.clear();
   if (!rarest.empty())
   {
      hoi4_traits.emplace(rarest);
      used_traits[rarest]++;
   }

   return hoi4_traits;
}
