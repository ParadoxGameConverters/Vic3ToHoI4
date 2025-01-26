#include "src/vic3_world/military/military_formation_importer.h"

#include <external/commonItems/CommonRegexes.h>
#include <external/commonItems/ParserHelpers.h>



vic3::MilitaryFormationImporter::MilitaryFormationImporter()
{
   military_formation_parser_.registerKeyword("country", [this](std::istream& input) {
      country_ = commonItems::getInt(input);
   });
   military_formation_parser_.registerKeyword("type", [this](std::istream& input) {
      const std::string type_string = commonItems::getString(input);
      if (type_string == "army")
      {
         type_ = MilitaryFormationType::kArmy;
      }
      else if (type_string == "fleet")
      {
         type_ = MilitaryFormationType::kFleet;
      }
   });
   military_formation_parser_.registerKeyword("name", [this](std::istream& input) {
      const std::string possible_name = commonItems::getString(input);
      if (!possible_name.empty())
      {
         name_ = possible_name;
      }
   });
   military_formation_parser_.registerKeyword("ordinal_number", [this](std::istream& input) {
      ordinal_number_ = commonItems::getInt(input);
   });
   military_formation_parser_.registerKeyword("building_to_expected_units_map", [this](std::istream& input) {
      building_to_expected_unit_parser_.parseStream(input);
   });
   military_formation_parser_.IgnoreUnregisteredItems();

   building_to_expected_unit_parser_.registerRegex(commonItems::integerRegex,
       [this]([[maybe_unused]] const std::string& unused, std::istream& input) {
          building_parser_.parseStream(input);
       });

   building_parser_.registerKeyword("unit_types_num_list", [this](std::istream& input) {
      for (const auto& [unit_type, number_string]: commonItems::assignments{input}.getAssignments())
      {
         try
         {
            units_[unit_type] += std::stoi(number_string);
         }
         catch (...)
         {
            continue;
         }
      }
   });
}


std::optional<vic3::MilitaryFormation> vic3::MilitaryFormationImporter::ImportMilitaryFormation(std::istream& input)
{
   country_.reset();
   type_ = MilitaryFormationType::kArmy;
   name_.reset();
   ordinal_number_.reset();
   units_.clear();

   military_formation_parser_.parseStream(input);

   if (country_.has_value())
   {
      return MilitaryFormation{.country = *country_,
          .type = type_,
          .name = name_,
          .ordinal_number = ordinal_number_,
          .units = units_};
   }

   return std::nullopt;
}