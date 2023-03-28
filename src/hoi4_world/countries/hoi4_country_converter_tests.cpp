#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/hoi4_world/countries/hoi4_country_converter.h"
#include "src/mappers/country/country_mapper.h"
#include "src/vic3_world/countries/vic3_country.h"



namespace hoi4
{

TEST(Hoi4worldCountriesCountryConverter, TagIsFromSourceCountry)
{
   const mappers::CountryMapper country_mapper({{"TAG", "T00"}, {"TWO", "T01"}});
   const vic3::Country source_country_one({.tag = "TAG", .color = commonItems::Color{std::array{1, 2, 3}}});
   const vic3::Country source_country_two({.tag = "TWO", .color = commonItems::Color{std::array{2, 4, 6}}});

   const auto country_one = ConvertCountry(source_country_one, {}, country_mapper, {}, {}, {}, {}, {}, {});
   const auto country_two = ConvertCountry(source_country_two, {}, country_mapper, {}, {}, {}, {}, {}, {});

   ASSERT_TRUE(country_one.has_value());
   EXPECT_EQ(country_one->GetTag(), "T00");
   EXPECT_EQ(country_one->GetColor(), commonItems::Color(std::array{1, 2, 3}));
   ASSERT_TRUE(country_two.has_value());
   EXPECT_EQ(country_two->GetTag(), "T01");
   EXPECT_EQ(country_two->GetColor(), commonItems::Color(std::array{2, 4, 6}));
}


TEST(Hoi4worldCountriesCountryConverter, NoCountryIfNoSourceTag)
{
   const mappers::CountryMapper country_mapper(std::map<std::string, std::string>{
       {"TAG", "T00"},
   });
   const vic3::Country source_country_one;

   const auto country_one = ConvertCountry(source_country_one, {}, country_mapper, {}, {}, {}, {}, {}, {});

   EXPECT_EQ(country_one, std::nullopt);
}



TEST(Hoi4worldCountriesCountryConverter, NoCountryIfNoTagMapping)
{
   const mappers::CountryMapper country_mapper;
   const vic3::Country source_country_one({.tag = "TAG"});

   const auto country_one = ConvertCountry(source_country_one, {}, country_mapper, {}, {}, {}, {}, {}, {});

   EXPECT_EQ(country_one, std::nullopt);
}


TEST(Hoi4worldCountriesCountryConverter, CapitalStatesAreConverted)
{
   const mappers::CountryMapper country_mapper({{"TAG", "TAG"}, {"TWO", "TWO"}});
   const vic3::Country source_country_one({.tag = "TAG", .capital_state = 2});
   const vic3::Country source_country_two({.tag = "TWO", .capital_state = 3});

   const std::map<int, int> vic3_state_ids_to_hoi4_state_ids{{2, 4}, {3, 9}};

   const auto country_one =
       ConvertCountry(source_country_one, {}, country_mapper, vic3_state_ids_to_hoi4_state_ids, {}, {}, {}, {}, {});
   const auto country_two =
       ConvertCountry(source_country_two, {}, country_mapper, vic3_state_ids_to_hoi4_state_ids, {}, {}, {}, {}, {});

   ASSERT_TRUE(country_one.has_value());
   EXPECT_EQ(country_one->GetCapitalState(), std::optional(4));
   ASSERT_TRUE(country_two.has_value());
   EXPECT_EQ(country_two->GetCapitalState(), std::optional(9));
}


TEST(Hoi4worldCountriesCountryConverter, NoCapitalStateIfNoSourceCapitalState)
{
   const mappers::CountryMapper country_mapper({{"TAG", "TAG"}, {"TWO", "TWO"}});
   const vic3::Country source_country_one({
       .tag = "TAG",
   });

   const std::map<int, int> vic3_state_ids_to_hoi4_state_ids{{2, 4}};

   const auto country_one =
       ConvertCountry(source_country_one, {}, country_mapper, vic3_state_ids_to_hoi4_state_ids, {}, {}, {}, {}, {});

   ASSERT_TRUE(country_one.has_value());
   EXPECT_EQ(country_one->GetCapitalState(), std::nullopt);
}


TEST(Hoi4worldCountriesCountryConverter, NoCapitalStateIfNoStateMapping)
{
   const mappers::CountryMapper country_mapper({{"TAG", "TAG"}, {"TWO", "TWO"}});
   const vic3::Country source_country_one({.tag = "TAG", .capital_state = 2});

   const auto country_one = ConvertCountry(source_country_one, {}, country_mapper, {}, {}, {}, {}, {}, {});

   ASSERT_TRUE(country_one.has_value());
   EXPECT_EQ(country_one->GetCapitalState(), std::nullopt);
}


TEST(Hoi4worldCountriesCountryConverter, TechnologiesAreConverted)
{
   const mappers::CountryMapper country_mapper({{"TAG", "TAG"}, {"TWO", "TWO"}});
   const vic3::Country source_country_one({.tag = "TAG", .capital_state = 2});

   const auto country_one = ConvertCountry(source_country_one,
       {"source_tech"},
       country_mapper,
       {},
       {{{"source_tech"}, std::nullopt, {"dest_tech_one", "dest_tech_two"}}},
       {},
       {},
       {},
       {});

   ASSERT_TRUE(country_one.has_value());
   EXPECT_THAT(country_one->GetTechnologies().GetTechnologies(),
       testing::UnorderedElementsAre(
           testing::Pair(std::nullopt, std::set<std::string>{"dest_tech_one", "dest_tech_two"})));
}


TEST(Hoi4worldCountriesCountryConverter, VariantsRequireAllRequiredTechs)
{
   const mappers::CountryMapper country_mapper({{"TAG", "TAG"}, {"TWO", "TWO"}});
   const vic3::Country source_country_one({.tag = "TAG", .capital_state = 2});

   const auto country_one = ConvertCountry(source_country_one,
       {"source_tech"},
       country_mapper,
       {},
       {{{"source_tech"}, std::nullopt, {"required_tech_one", "required_tech_two"}}},
       {
           EquipmentVariant({}, {}, {{"name", "legacy_ship: no_required_techs_automatically_succeeds"}}),
           EquipmentVariant({"required_tech_missing"}, {}, {{"name", "legacy_ship: missing_required_tech_fails"}}),
           EquipmentVariant({"required_tech_one"}, {}, {{"name", "legacy_ship: present_required_tech_succeeds"}}),
           EquipmentVariant({"required_tech_one", "required_tech_missing"},
               {},
               {{"name", "legacy_ship: missing_and_present_required_techs_fails"}}),
           EquipmentVariant({"required_tech_one", "required_tech_two"},
               {},
               {{"name", "legacy_ship: all_present_required_techs_succeeds"}}),
       },
       {
           EquipmentVariant({}, {}, {{"name", "ship: no_required_techs_automatically_succeeds"}}),
           EquipmentVariant({"required_tech_missing"}, {}, {{"name", "ship: missing_required_tech_fails"}}),
           EquipmentVariant({"required_tech_one"}, {}, {{"name", "ship: present_required_tech_succeeds"}}),
           EquipmentVariant({"required_tech_one", "required_tech_missing"},
               {},
               {{"name", "ship: missing_and_present_required_techs_fails"}}),
           EquipmentVariant({"required_tech_one", "required_tech_two"},
               {},
               {{"name", "ship: all_present_required_techs_succeeds"}}),
       },
       {
           EquipmentVariant({}, {}, {{"name", "plane: no_required_techs_automatically_succeeds"}}),
           EquipmentVariant({"required_tech_missing"}, {}, {{"name", "plane: missing_required_tech_fails"}}),
           EquipmentVariant({"required_tech_one"}, {}, {{"name", "plane: present_required_tech_succeeds"}}),
           EquipmentVariant({"required_tech_one", "required_tech_missing"},
               {},
               {{"name", "plane: missing_and_present_required_techs_fails"}}),
           EquipmentVariant({"required_tech_one", "required_tech_two"},
               {},
               {{"name", "plane: all_present_required_techs_succeeds"}}),
       },
       {
           EquipmentVariant({}, {}, {{"name", "tank: no_required_techs_automatically_succeeds"}}),
           EquipmentVariant({"required_tech_missing"}, {}, {{"name", "tank: missing_required_tech_fails"}}),
           EquipmentVariant({"required_tech_one"}, {}, {{"name", "tank: present_required_tech_succeeds"}}),
           EquipmentVariant({"required_tech_one", "required_tech_missing"},
               {},
               {{"name", "tank: missing_and_present_required_techs_fails"}}),
           EquipmentVariant({"required_tech_one", "required_tech_two"},
               {},
               {{"name", "tank: all_present_required_techs_succeeds"}}),
       });

   ASSERT_TRUE(country_one.has_value());
   EXPECT_THAT(country_one->GetLegacyShipVariants(),
       testing::ElementsAre(
           EquipmentVariant({}, {}, {{"name", "legacy_ship: no_required_techs_automatically_succeeds"}}),
           EquipmentVariant({"required_tech_one"}, {}, {{"name", "legacy_ship: present_required_tech_succeeds"}}),
           EquipmentVariant({"required_tech_one", "required_tech_two"},
               {},
               {{"name", "legacy_ship: all_present_required_techs_succeeds"}})));
   EXPECT_THAT(country_one->GetShipVariants(),
       testing::ElementsAre(EquipmentVariant({}, {}, {{"name", "ship: no_required_techs_automatically_succeeds"}}),
           EquipmentVariant({"required_tech_one"}, {}, {{"name", "ship: present_required_tech_succeeds"}}),
           EquipmentVariant({"required_tech_one", "required_tech_two"},
               {},
               {{"name", "ship: all_present_required_techs_succeeds"}})));
   EXPECT_THAT(country_one->GetPlaneVariants(),
       testing::ElementsAre(EquipmentVariant({}, {}, {{"name", "plane: no_required_techs_automatically_succeeds"}}),
           EquipmentVariant({"required_tech_one"}, {}, {{"name", "plane: present_required_tech_succeeds"}}),
           EquipmentVariant({"required_tech_one", "required_tech_two"},
               {},
               {{"name", "plane: all_present_required_techs_succeeds"}})));
   EXPECT_THAT(country_one->GetTankVariants(),
       testing::ElementsAre(EquipmentVariant({}, {}, {{"name", "tank: no_required_techs_automatically_succeeds"}}),
           EquipmentVariant({"required_tech_one"}, {}, {{"name", "tank: present_required_tech_succeeds"}}),
           EquipmentVariant({"required_tech_one", "required_tech_two"},
               {},
               {{"name", "tank: all_present_required_techs_succeeds"}})));
}


TEST(Hoi4worldCountriesCountryConverter, VariantsBlockedByAnyBlockingTechs)
{
   const mappers::CountryMapper country_mapper({{"TAG", "TAG"}, {"TWO", "TWO"}});
   const vic3::Country source_country_one({.tag = "TAG", .capital_state = 2});

   const auto country_one = ConvertCountry(source_country_one,
       {"source_tech"},
       country_mapper,
       {},
       {{{"source_tech"}, std::nullopt, {"blocking_tech_one", "blocking_tech_two"}}},
       {
           EquipmentVariant({}, {}, {{"name", "legacy_ship: no_blocking_techs_automatically_succeeds"}}),
           EquipmentVariant({}, {"blocking_tech_one"}, {{"name", "legacy_ship: having_blocking_tech_fails"}}),
           EquipmentVariant({}, {"blocking_tech_missing"}, {{"name", "legacy_ship: missing_blocking_tech_succeeds"}}),
           EquipmentVariant({},
               {"blocking_tech_one", "blocking_tech_missing"},
               {{"name", "legacy_ship: missing_and_present_blocking_techs_fails"}}),
       },
       {
           EquipmentVariant({}, {}, {{"name", "ship: no_blocking_techs_automatically_succeeds"}}),
           EquipmentVariant({}, {"blocking_tech_one"}, {{"name", "ship: having_blocking_tech_fails"}}),
           EquipmentVariant({}, {"blocking_tech_missing"}, {{"name", "ship: missing_blocking_tech_succeeds"}}),
           EquipmentVariant({},
               {"blocking_tech_one", "blocking_tech_missing"},
               {{"name", "ship: missing_and_present_blocking_techs_fails"}}),
       },
       {
           EquipmentVariant({}, {}, {{"name", "plane: no_blocking_techs_automatically_succeeds"}}),
           EquipmentVariant({}, {"blocking_tech_one"}, {{"name", "plane: having_blocking_tech_fails"}}),
           EquipmentVariant({}, {"blocking_tech_missing"}, {{"name", "plane: missing_blocking_tech_succeeds"}}),
           EquipmentVariant({},
               {"blocking_tech_one", "blocking_tech_missing"},
               {{"name", "plane: missing_and_present_blocking_techs_fails"}}),
       },
       {
           EquipmentVariant({}, {}, {{"name", "tank: no_blocking_techs_automatically_succeeds"}}),
           EquipmentVariant({}, {"blocking_tech_one"}, {{"name", "tank: having_blocking_tech_fails"}}),
           EquipmentVariant({}, {"blocking_tech_missing"}, {{"name", "tank: missing_blocking_tech_succeeds"}}),
           EquipmentVariant({},
               {"blocking_tech_one", "blocking_tech_missing"},
               {{"name", "tank: missing_and_present_blocking_techs_fails"}}),
       });

   ASSERT_TRUE(country_one.has_value());
   EXPECT_THAT(country_one->GetLegacyShipVariants(),
       testing::ElementsAre(
           EquipmentVariant({}, {}, {{"name", "legacy_ship: no_blocking_techs_automatically_succeeds"}}),
           EquipmentVariant({}, {"blocking_tech_missing"}, {{"name", "legacy_ship: missing_blocking_tech_succeeds"}})));
   EXPECT_THAT(country_one->GetShipVariants(),
       testing::ElementsAre(EquipmentVariant({}, {}, {{"name", "ship: no_blocking_techs_automatically_succeeds"}}),
           EquipmentVariant({}, {"blocking_tech_missing"}, {{"name", "ship: missing_blocking_tech_succeeds"}})));
   EXPECT_THAT(country_one->GetPlaneVariants(),
       testing::ElementsAre(EquipmentVariant({}, {}, {{"name", "plane: no_blocking_techs_automatically_succeeds"}}),
           EquipmentVariant({}, {"blocking_tech_missing"}, {{"name", "plane: missing_blocking_tech_succeeds"}})));
   EXPECT_THAT(country_one->GetTankVariants(),
       testing::ElementsAre(EquipmentVariant({}, {}, {{"name", "tank: no_blocking_techs_automatically_succeeds"}}),
           EquipmentVariant({}, {"blocking_tech_missing"}, {{"name", "tank: missing_blocking_tech_succeeds"}})));
}

}  // namespace hoi4