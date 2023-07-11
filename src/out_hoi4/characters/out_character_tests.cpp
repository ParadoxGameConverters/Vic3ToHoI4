#include <filesystem>
#include <sstream>

#include "external/commonItems/OSCompatibilityLayer.h"
#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/out_hoi4/characters/out_character.h"


namespace out
{


TEST(Outhoi4Characters, GeneralsAreOutput)
{
   const hoi4::Character character({
       .id = 1,
       .first_name = "Test",
       .last_name = "Mann",
       .portrait_alias = "GFX_test",
       .general_data = std::make_optional<hoi4::General>(hoi4::General{.traits = {"trait0", "trait1"}}),
   });

   std::stringstream out;
   OutputCharacter(out, "TAG", character);

   EXPECT_EQ(out.str(),
       "\tTAG_1 = {\n"
       "\t\tname = Test_Mann\n"
       "\t\tportraits = {\n"
       "\t\t\tarmy = {\n"
       "\t\t\t\tlarge = GFX_test\n"
       "\t\t\t\tsmall = GFX_test_small\n"
       "\t\t\t}\n"
       "\t\t}\n"
       "\t\tcorps_commander = {\n"
       "\t\t\ttraits = { trait0 trait1 }\n"
       "\t\t\tskill = 1\n"
       "\t\t\tattack_skill = 1\n"
       "\t\t\tdefense_skill = 1\n"
       "\t\t\tplanning_skill = 1\n"
       "\t\t\tlogistics_skill = 1\n"
       "\t\t}\n"
       "\t}\n");
}


TEST(Outhoi4Characters, FieldMarshalsAreOutput)
{
   const hoi4::Character character({
       .id = 1,
       .first_name = "Test",
       .last_name = "Mann",
       .portrait_alias = "GFX_test",
       .general_data =
           std::make_optional<hoi4::General>(hoi4::General{.traits = {"trait0", "trait1"}, .is_field_marshal = true}),
   });

   std::stringstream out;
   OutputCharacter(out, "TAG", character);

   EXPECT_EQ(out.str(),
       "\tTAG_1 = {\n"
       "\t\tname = Test_Mann\n"
       "\t\tportraits = {\n"
       "\t\t\tarmy = {\n"
       "\t\t\t\tlarge = GFX_test\n"
       "\t\t\t\tsmall = GFX_test_small\n"
       "\t\t\t}\n"
       "\t\t}\n"
       "\t\tfield_marshal = {\n"
       "\t\t\ttraits = { trait0 trait1 }\n"
       "\t\t\tskill = 1\n"
       "\t\t\tattack_skill = 1\n"
       "\t\t\tdefense_skill = 1\n"
       "\t\t\tplanning_skill = 1\n"
       "\t\t\tlogistics_skill = 1\n"
       "\t\t}\n"
       "\t}\n");
}


TEST(Outhoi4Characters, AdmiralsAreOutput)
{
   const hoi4::Character character({
       .id = 1,
       .first_name = "Test",
       .last_name = "Mann",
       .portrait_alias = "GFX_test",
       .admiral_data = std::make_optional<hoi4::Admiral>(hoi4::Admiral{.traits = {"trait0", "trait1"}}),
   });

   std::stringstream out;
   OutputCharacter(out, "TAG", character);

   EXPECT_EQ(out.str(),
       "\tTAG_1 = {\n"
       "\t\tname = Test_Mann\n"
       "\t\tportraits = {\n"
       "\t\t\tnavy = {\n"
       "\t\t\t\tlarge = GFX_test\n"
       "\t\t\t\tsmall = GFX_test_small\n"
       "\t\t\t}\n"
       "\t\t}\n"
       "\t\tnavy_leader = {\n"
       "\t\t\ttraits = { trait0 trait1 }\n"
       "\t\t\tskill = 1\n"
       "\t\t\tattack_skill = 1\n"
       "\t\t\tdefense_skill = 1\n"
       "\t\t\tmaneuvering_skill = 1\n"
       "\t\t\tcoordination_skill = 1\n"
       "\t\t}\n"
       "\t}\n");
}


TEST(Outhoi4Characters, CountryLeadersAreOutput)
{
   const hoi4::Character character({
       .id = 1,
       .first_name = "Test",
       .last_name = "Mann",
       .portrait_alias = "GFX_test",
       .leader_data =
           std::make_optional<hoi4::Leader>(hoi4::Leader{.sub_ideology = "socialist", .traits = {"trait0", "trait1"}}),
   });

   std::stringstream out;
   OutputCharacter(out, "TAG", character);

   EXPECT_EQ(out.str(),
       "\tTAG_1 = {\n"
       "\t\tname = Test_Mann\n"
       "\t\tportraits = {\n"
       "\t\t\tcivilian = {\n"
       "\t\t\t\tlarge = GFX_test\n"
       "\t\t\t}\n"
       "\t\t}\n"
       "\t\tcountry_leader = {\n"
       "\t\t\tideology = socialist\n"
       "\t\t\ttraits = { trait0 trait1 }\n"
       "\t\t}\n"
       "\t}\n");
}


TEST(Outhoi4Characters, AdvisorsAreOutput)
{
   const hoi4::Character character({
       .id = 1,
       .first_name = "Test",
       .last_name = "Mann",
       .portrait_alias = "GFX_test",
       .advisor_data = std::make_optional<hoi4::Advisor>(
           hoi4::Advisor{.traits = {"trait0", "trait1"}, .slot = "theorist", .ledger = "army"}),
   });

   std::stringstream out;
   OutputCharacter(out, "TAG", character);

   EXPECT_EQ(out.str(),
       "\tTAG_1 = {\n"
       "\t\tname = Test_Mann\n"
       "\t\tportraits = {\n"
       "\t\t\tcivilian = {\n"
       "\t\t\t\tsmall = GFX_test\n"
       "\t\t\t}\n"
       "\t\t}\n"
       "\t\tadvisor = {\n"
       "\t\t\ttraits = { trait0 trait1 }\n"
       "\t\t\tslot = theorist\n"
       "\t\t\tledger = army\n"
       "\t\t\tidea_token = i_1\n"
       "\t\t\tcost = 150\n"
       "\t\t}\n"
       "\t}\n");
}


TEST(Outhoi4Characters, LedgerlessAdvisorsAreOutputWithoutLedgers)
{
   const hoi4::Character character({
       .id = 1,
       .first_name = "Test",
       .last_name = "Mann",
       .portrait_alias = "GFX_test",
       .advisor_data = std::make_optional<hoi4::Advisor>(
           hoi4::Advisor{.traits = {"trait0", "trait1"}, .slot = "political_advisor"}),
   });

   std::stringstream out;
   OutputCharacter(out, "TAG", character);

   EXPECT_EQ(out.str(),
       "\tTAG_1 = {\n"
       "\t\tname = Test_Mann\n"
       "\t\tportraits = {\n"
       "\t\t\tcivilian = {\n"
       "\t\t\t\tsmall = GFX_test\n"
       "\t\t\t}\n"
       "\t\t}\n"
       "\t\tadvisor = {\n"
       "\t\t\ttraits = { trait0 trait1 }\n"
       "\t\t\tslot = political_advisor\n"
       "\t\t\tidea_token = i_1\n"
       "\t\t\tcost = 150\n"
       "\t\t}\n"
       "\t}\n");
}


TEST(Outhoi4Characters, GeneralAdvisorsHaveArmyPortraits)
{
   const hoi4::Character character({
       .id = 1,
       .first_name = "Test",
       .last_name = "Mann",
       .portrait_alias = "GFX_test",
       .general_data = std::make_optional<hoi4::General>(hoi4::General{.traits = {"trait0", "trait1"}}),
       .advisor_data = std::make_optional<hoi4::Advisor>(
           hoi4::Advisor{.traits = {"trait2", "trait3"}, .slot = "theorist", .ledger = "army"}),
   });

   std::stringstream out;
   OutputCharacter(out, "TAG", character);

   EXPECT_EQ(out.str(),
       "\tTAG_1 = {\n"
       "\t\tname = Test_Mann\n"
       "\t\tportraits = {\n"
       "\t\t\tarmy = {\n"
       "\t\t\t\tlarge = GFX_test\n"
       "\t\t\t\tsmall = GFX_test_small\n"
       "\t\t\t}\n"
       "\t\t\tcivilian = {\n"
       "\t\t\t}\n"
       "\t\t}\n"
       "\t\tcorps_commander = {\n"
       "\t\t\ttraits = { trait0 trait1 }\n"
       "\t\t\tskill = 1\n"
       "\t\t\tattack_skill = 1\n"
       "\t\t\tdefense_skill = 1\n"
       "\t\t\tplanning_skill = 1\n"
       "\t\t\tlogistics_skill = 1\n"
       "\t\t}\n"
       "\t\tadvisor = {\n"
       "\t\t\ttraits = { trait2 trait3 }\n"
       "\t\t\tslot = theorist\n"
       "\t\t\tledger = army\n"
       "\t\t\tidea_token = i_1\n"
       "\t\t\tcost = 150\n"
       "\t\t}\n"
       "\t}\n");
}


TEST(Outhoi4Characters, AdmiralAdvisorsHaveNavyPortraits)
{
   const hoi4::Character character({
       .id = 1,
       .first_name = "Test",
       .last_name = "Mann",
       .portrait_alias = "GFX_test",
       .admiral_data = std::make_optional<hoi4::Admiral>(hoi4::Admiral{.traits = {"trait0", "trait1"}}),
       .advisor_data = std::make_optional<hoi4::Advisor>(
           hoi4::Advisor{.traits = {"trait2", "trait3"}, .slot = "theorist", .ledger = "navy"}),
   });

   std::stringstream out;
   OutputCharacter(out, "TAG", character);

   EXPECT_EQ(out.str(),
       "\tTAG_1 = {\n"
       "\t\tname = Test_Mann\n"
       "\t\tportraits = {\n"
       "\t\t\tnavy = {\n"
       "\t\t\t\tlarge = GFX_test\n"
       "\t\t\t\tsmall = GFX_test_small\n"
       "\t\t\t}\n"
       "\t\t\tcivilian = {\n"
       "\t\t\t}\n"
       "\t\t}\n"
       "\t\tnavy_leader = {\n"
       "\t\t\ttraits = { trait0 trait1 }\n"
       "\t\t\tskill = 1\n"
       "\t\t\tattack_skill = 1\n"
       "\t\t\tdefense_skill = 1\n"
       "\t\t\tmaneuvering_skill = 1\n"
       "\t\t\tcoordination_skill = 1\n"
       "\t\t}\n"
       "\t\tadvisor = {\n"
       "\t\t\ttraits = { trait2 trait3 }\n"
       "\t\t\tslot = theorist\n"
       "\t\t\tledger = navy\n"
       "\t\t\tidea_token = i_1\n"
       "\t\t\tcost = 150\n"
       "\t\t}\n"
       "\t}\n");
}


TEST(Outhoi4Characters, WomenAreOutput)
{
   const hoi4::Character character({
       .id = 1,
       .first_name = "Test",
       .last_name = "WoMann",
       .is_female = true,
   });

   std::stringstream out;
   OutputCharacter(out, "TAG", character);

   EXPECT_EQ(out.str(),
       "\tTAG_1 = {\n"
       "\t\tname = Test_WoMann\n"
       "\t\tgender = female\n"
       "\t\tportraits = {\n"
       "\t\t}\n"
       "\t}\n");
}


TEST(Outhoi4Characters, MaleSpiesAreOutput)
{
   const hoi4::Character character({
       .id = 1,
       .first_name = "Test",
       .last_name = "Mann",
       .portrait_alias = "GFX_test",
       .spy_data =
           std::make_optional<hoi4::Spy>(hoi4::Spy{.traits = {"trait2", "trait3"}, .nationalities = {"TAG", "TWO"}}),
   });

   std::stringstream out;
   OutputSpy(out, character);

   EXPECT_EQ(out.str(),
       "\tcreate_operative_leader = {\n"
       "\t\tname = Test_Mann\n"
       "\t\tGFX = GFX_test\n"
       "\t\ttraits = { trait2 trait3 }\n"
       "\t\tbypass_recruitment = no\n"
       "\t\tavailable_to_spy_master = yes\n"
       "\t\tnationalities = { TAG TWO }\n"
       "\t}\n");
}


TEST(Outhoi4Characters, FemaleSpiesAreOutput)
{
   const hoi4::Character character({
       .id = 1,
       .first_name = "Test",
       .last_name = "WoMann",
       .portrait_alias = "GFX_test",
       .is_female = true,
       .spy_data =
           std::make_optional<hoi4::Spy>(hoi4::Spy{.traits = {"trait2", "trait3"}, .nationalities = {"TAG", "TWO"}}),
   });

   std::stringstream out;
   OutputSpy(out, character);

   EXPECT_EQ(out.str(),
       "\tcreate_operative_leader = {\n"
       "\t\tname = Test_WoMann\n"
       "\t\tGFX = GFX_test\n"
       "\t\ttraits = { trait2 trait3 }\n"
       "\t\tbypass_recruitment = no\n"
       "\t\tavailable_to_spy_master = yes\n"
       "\t\tfemale = yes\n"
       "\t\tnationalities = { TAG TWO }\n"
       "\t}\n");
}


TEST(Outhoi4Characters, MissingSpiesAreLogged)
{
   const hoi4::Character character({
       .id = 1,
       .first_name = "Test",
       .last_name = "Mann",
       .portrait_alias = "GFX_test",
   });

   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   std::stringstream out;
   OutputSpy(out, character);

   std::cout.rdbuf(cout_buffer);

   EXPECT_THAT(log.str(), testing::HasSubstr(R"([ERROR] Attempted to output spy with no spy data.)"));
}


TEST(Outhoi4Characters, CharactersWithoutPortraitGetUnknownPortrait)
{
   const hoi4::Character character({
       .id = 1,
       .first_name = "Test",
       .last_name = "Mann",
       .general_data = std::make_optional<hoi4::General>(hoi4::General{.traits = {"trait0", "trait1"}}),
   });

   std::stringstream out;
   OutputCharacter(out, "TAG", character);

   EXPECT_EQ(out.str(),
       "\tTAG_1 = {\n"
       "\t\tname = Test_Mann\n"
       "\t\tportraits = {\n"
       "\t\t\tarmy = {\n"
       "\t\t\t\tlarge = GFX_portrait_unknown\n"
       "\t\t\t\tsmall = GFX_portrait_unknown_small\n"
       "\t\t\t}\n"
       "\t\t}\n"
       "\t\tcorps_commander = {\n"
       "\t\t\ttraits = { trait0 trait1 }\n"
       "\t\t\tskill = 1\n"
       "\t\t\tattack_skill = 1\n"
       "\t\t\tdefense_skill = 1\n"
       "\t\t\tplanning_skill = 1\n"
       "\t\t\tlogistics_skill = 1\n"
       "\t\t}\n"
       "\t}\n");
}


TEST(Outhoi4Characters, SpiesWithoutPortraitGetUnknownPortrait)
{
   const hoi4::Character character({
       .id = 1,
       .first_name = "Test",
       .last_name = "Mann",
       .spy_data =
           std::make_optional<hoi4::Spy>(hoi4::Spy{.traits = {"trait2", "trait3"}, .nationalities = {"TAG", "TWO"}}),
   });

   std::stringstream out;
   OutputSpy(out, character);

   EXPECT_EQ(out.str(),
       "\tcreate_operative_leader = {\n"
       "\t\tname = Test_Mann\n"
       "\t\tGFX = GFX_portrait_operative_unknown\n"
       "\t\ttraits = { trait2 trait3 }\n"
       "\t\tbypass_recruitment = no\n"
       "\t\tavailable_to_spy_master = yes\n"
       "\t\tnationalities = { TAG TWO }\n"
       "\t}\n");
}

}  // namespace out