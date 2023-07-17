#ifndef SRC_MAPPERS_CHARACTER_CHARACTERTRAITMAPPER_H
#define SRC_MAPPERS_CHARACTER_CHARACTERTRAITMAPPER_H



#include <map>
#include <string>


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

struct GeneralTraitMapping
{
   std::set<hoi4::Trait> traits;
   int attack = 0;
   int defense = 0;
   int planning = 0;
   int logistics = 0;

   std::partial_ordering operator<=>(const GeneralTraitMapping&) const = default;
};
struct AdmiralTraitMapping
{
   std::set<hoi4::Trait> traits;
   int attack = 0;
   int defense = 0;
   int maneuvering = 0;
   int coordination = 0;

   std::partial_ordering operator<=>(const AdmiralTraitMapping&) const = default;
};
using GeneralTraitMap = std::map<vic3::Trait, GeneralTraitMapping>;
using AdmiralTraitMap = std::map<vic3::Trait, AdmiralTraitMapping>;
using SpyTraitMap = std::map<vic3::Trait, hoi4::Trait>;

class CharacterTraitMapper
{
  public:
   explicit CharacterTraitMapper(GeneralTraitMap general_trait_rules,
       AdmiralTraitMap admiral_trait_rules,
       SpyTraitMap spy_trait_rules):
       general_trait_rules_(std::move(general_trait_rules)),
       admiral_trait_rules_(std::move(admiral_trait_rules)),
       spy_trait_rules_(std::move(spy_trait_rules))
   {
   }

   [[nodiscard]] GeneralTraitMapping GetGeneralMappedData(const std::set<vic3::Trait>& source_traits) const;
   [[nodiscard]] AdmiralTraitMapping GetAdmiralMappedData(const std::set<vic3::Trait>& source_traits) const;
   [[nodiscard]] std::set<hoi4::Trait> GetSpyMappedTraits(const std::set<vic3::Trait>& source_traits) const;

  private:
   GeneralTraitMap general_trait_rules_;
   AdmiralTraitMap admiral_trait_rules_;
   SpyTraitMap spy_trait_rules_;
};

}  // namespace mappers



#endif  // SRC_MAPPERS_CHARACTER_LEADERTYPEMAPPER_H