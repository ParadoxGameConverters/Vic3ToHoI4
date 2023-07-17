#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/hoi4_world/characters/hoi4_character.h"
#include "src/mappers/character/character_trait_mapper.h"
#include "src/mappers/character/character_trait_mapper_importer.h"



namespace mappers
{

TEST(MappersCharacterCharactertraitmapper, NoMappingsReturnsBasicAdmirals)
{
   const CharacterTraitMapper character_trait_mapper =
       ImportCharacterTraitMapper("test_files/configurables/character_traits.txt");
   EXPECT_EQ(character_trait_mapper.GetAdmiralMappedData({}),
       hoi4::Admiral({
           .traits = {},
           .attack = 1,
           .defense = 1,
           .maneuvering = 1,
           .coordination = 1,
       }));
}


TEST(MappersCharacterCharactertraitmapper, NoMappingsReturnsBasicGenerals)
{
   const CharacterTraitMapper character_trait_mapper =
       ImportCharacterTraitMapper("test_files/configurables/character_traits.txt");
   EXPECT_EQ(character_trait_mapper.GetGeneralMappedData({}, false),
       hoi4::General({
           .traits = {},
           .is_field_marshal = false,
           .attack = 1,
           .defense = 1,
           .planning = 1,
           .logistics = 1,
       }));
}


TEST(MappersCharacterCharactertraitmapper, NoMappingsReturnsBasicFieldMarshals)
{
   const CharacterTraitMapper character_trait_mapper =
       ImportCharacterTraitMapper("test_files/configurables/character_traits.txt");
   EXPECT_EQ(character_trait_mapper.GetGeneralMappedData({}, true),
       hoi4::General({
           .traits = {},
           .is_field_marshal = true,
           .attack = 1,
           .defense = 1,
           .planning = 1,
           .logistics = 1,
       }));
}


TEST(MappersCharacterCharactertraitmapper, NoMappingsReturnsTraitlessSpies)
{
   const CharacterTraitMapper character_trait_mapper =
       ImportCharacterTraitMapper("test_files/configurables/character_traits.txt");
   EXPECT_TRUE(character_trait_mapper.GetSpyMappedTraits({}).empty());
}


TEST(MappersCharacterCharactertraitmapper, AdmiralDataIsMapped)
{
   const CharacterTraitMapper character_trait_mapper =
       ImportCharacterTraitMapper("test_files/configurables/character_traits.txt");
   EXPECT_EQ(character_trait_mapper.GetAdmiralMappedData({"bandit"}),
       hoi4::Admiral({
           .traits = {"naval_trait_0", "naval_trait_1"},
           .attack = 2,
           .defense = 2,
           .maneuvering = 2,
           .coordination = 2,
       }));
}


TEST(MappersCharacterCharactertraitmapper, GeneralDataIsMapped)
{
   const CharacterTraitMapper character_trait_mapper =
       ImportCharacterTraitMapper("test_files/configurables/character_traits.txt");
   const auto& a = character_trait_mapper.GetGeneralMappedData({"charismatic"}, false);
   EXPECT_EQ(character_trait_mapper.GetGeneralMappedData({"charismatic"}, false),
       hoi4::General({
           .traits = {"land_trait_0", "land_trait_1"},
           .is_field_marshal = false,
           .attack = 2,
           .defense = 2,
           .planning = 2,
           .logistics = 2,
       }));
}


TEST(MappersCharacterCharactertraitmapper, FieldMarshalDataIsMapped)
{
   const CharacterTraitMapper character_trait_mapper =
       ImportCharacterTraitMapper("test_files/configurables/character_traits.txt");
   EXPECT_EQ(character_trait_mapper.GetGeneralMappedData({"charismatic"}, true),
       hoi4::General({
           .traits = {"land_trait_0", "land_trait_1", "charismatic"},
           .is_field_marshal = true,
           .attack = 2,
           .defense = 2,
           .planning = 2,
           .logistics = 2,
       }));
}


TEST(MappersCharacterCharactertraitmapper, SpyDataIsMapped)
{
   const CharacterTraitMapper character_trait_mapper =
       ImportCharacterTraitMapper("test_files/configurables/character_traits.txt");
   EXPECT_THAT(character_trait_mapper.GetSpyMappedTraits({"brave", "persistent"}),
       testing::UnorderedElementsAre("operative_tough", "operative_commando"));
}

}  // namespace mappers