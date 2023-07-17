#include "character_trait_mapper.h"

#include "external/fmt/include/fmt/format.h"

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
