#include "character_trait_mapper.h"

mappers::AdmiralTraitMapping mappers::CharacterTraitMapper::GetAdmiralMappedData(
    const std::set<vic3::Trait>& source_traits) const
{
   return AdmiralTraitMapping();
}

mappers::GeneralTraitMapping mappers::CharacterTraitMapper::GetGeneralMappedData(
    const std::set<vic3::Trait>& source_traits,
    bool is_field_marshal) const
{
   return GeneralTraitMapping();
}

std::set<hoi4::Trait> mappers::CharacterTraitMapper::GetSpyMappedTraits(
    const std::set<vic3::Trait>& source_traits) const
{
   return std::set<hoi4::Trait>();
}
