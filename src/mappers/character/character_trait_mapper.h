#ifndef SRC_MAPPERS_CHARACTER_CHARACTERTRAITMAPPER_H
#define SRC_MAPPERS_CHARACTER_CHARACTERTRAITMAPPER_H



#include <map>
#include <string>

#include "src/hoi4_world/characters/hoi4_character.h"


namespace hoi4
{
using Trait = std::string;
}
namespace vic3
{
using Trait = std::string;
}

namespace mappers
{

struct AdmiralTraitMapping
{
   std::set<hoi4::Trait> traits;
   int attack = 0;
   int defense = 0;
   int maneuvering = 0;
   int coordination = 0;

   std::partial_ordering operator<=>(const AdmiralTraitMapping&) const = default;
};
struct GeneralTraitMapping
{
   std::set<hoi4::Trait> traits;
   std::set<hoi4::Trait> field_marshal_traits;
   int attack = 0;
   int defense = 0;
   int planning = 0;
   int logistics = 0;

   std::partial_ordering operator<=>(const GeneralTraitMapping&) const = default;
};
using AdmiralTraitMap = std::map<vic3::Trait, AdmiralTraitMapping>;
using GeneralTraitMap = std::map<vic3::Trait, GeneralTraitMapping>;
using SpyTraitMap = std::map<vic3::Trait, hoi4::Trait>;
using AdvisorTraitMap = std::map<vic3::Trait, hoi4::Trait>;

class CharacterTraitMapper
{
  public:
   explicit CharacterTraitMapper(AdmiralTraitMap admiral_trait_rules,
       GeneralTraitMap general_trait_rules,
       SpyTraitMap spy_trait_rules,
       AdvisorTraitMap advisor_trait_rules):
       admiral_trait_rules_(std::move(admiral_trait_rules)),
       general_trait_rules_(std::move(general_trait_rules)),
       spy_trait_rules_(std::move(spy_trait_rules)),
       advisor_trait_rules_(std::move(advisor_trait_rules))
   {
   }

   [[nodiscard]] hoi4::Admiral GetAdmiralMappedData(const std::set<vic3::Trait>& source_traits) const;
   [[nodiscard]] hoi4::General GetGeneralMappedData(const std::set<vic3::Trait>& source_traits,
       bool is_field_marshal) const;
   [[nodiscard]] std::set<hoi4::Trait> GetSpyMappedTraits(const std::set<vic3::Trait>& source_traits) const;
   [[nodiscard]] std::set<hoi4::Trait> GetAdvisorMappedTraits(const std::set<vic3::Trait>& source_traits) const;

  private:
   AdmiralTraitMap admiral_trait_rules_;
   GeneralTraitMap general_trait_rules_;
   SpyTraitMap spy_trait_rules_;
   AdvisorTraitMap advisor_trait_rules_;
};

}  // namespace mappers



#endif  // SRC_MAPPERS_CHARACTER_LEADERTYPEMAPPER_H
