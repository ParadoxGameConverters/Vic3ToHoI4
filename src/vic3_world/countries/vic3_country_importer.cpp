#include "src/vic3_world/countries/vic3_country_importer.h"

#include "external/commonItems/CommonRegexes.h"
#include "external/commonItems/Log.h"
#include "external/commonItems/ParserHelpers.h"
#include "external/commonItems/StringUtils.h"
#include "external/fmt/include/fmt/format.h"



namespace
{

vic3::BudgetLevel parseBudgetLevel(const std::string& level_string)
{
   if (level_string == "very_low")
   {
      return vic3::BudgetLevel::VeryLow;
   }
   if (level_string == "low")
   {
      return vic3::BudgetLevel::Low;
   }
   if (level_string == "medium")
   {
      return vic3::BudgetLevel::Medium;
   }
   if (level_string == "high")
   {
      return vic3::BudgetLevel::High;
   }
   if (level_string == "very_high")
   {
      return vic3::BudgetLevel::VeryHigh;
   }

   Log(LogLevel::Error) << fmt::format("Unknown budget level {}", level_string);
   return vic3::BudgetLevel::Medium;
}

}  // namespace


vic3::CountryImporter::CountryImporter()
{
   dynamic_name_parser_.registerKeyword("dynamic_country_name", [this](std::istream& input_stream) {
      options_.dynamic_name = commonItems::getString(input_stream);
   });
   dynamic_name_parser_.registerKeyword("dynamic_country_adjective", [this](std::istream& input_stream) {
      options_.dynamic_adjective = commonItems::getString(input_stream);
   });
   dynamic_name_parser_.registerKeyword("use_overlord_prefix", [this](std::istream& input_stream) {
      options_.use_overlord_prefix = (commonItems::getString(input_stream) == "yes");
   });

   country_parser_.registerKeyword("definition", [this](std::istream& input_stream) {
      options_.tag = commonItems::getString(input_stream);
   });
   country_parser_.registerKeyword("dynamic_country_name", [this](std::istream& input_stream) {
      options_.dynamic_name = commonItems::getString(input_stream);
   });
   country_parser_.registerKeyword("dynamic_country_adjective", [this](std::istream& input_stream) {
      options_.dynamic_adjective = commonItems::getString(input_stream);
   });
   country_parser_.registerKeyword("dynamic_name", [this](std::istream& input_stream) {
      dynamic_name_parser_.parseStream(input_stream);
   });
   country_parser_.registerKeyword("map_color", [this](std::istream& input_stream) {
      options_.color = commonItems::Color::Factory{}.getColor(input_stream);
   });
   country_parser_.registerKeyword("capital", [this](std::istream& input_stream) {
      const int64_t temp_number = commonItems::getLlong(input_stream);
      if (temp_number == 4294967295)
      {
         options_.capital_state = -1;
      }
      else
      {
         options_.capital_state = static_cast<int>(temp_number);
      }
   });
   country_parser_.registerKeyword("country_type", [this](std::istream& input_stream) {
      options_.country_type = commonItems::getString(input_stream);
   });
   country_parser_.registerKeyword("tax_level", [this](std::istream& input_stream) {
      options_.tax_level = parseBudgetLevel(commonItems::getString(input_stream));
   });
   country_parser_.registerKeyword("salaries", [this](std::istream& input_stream) {
      options_.salary_level = parseBudgetLevel(commonItems::getString(input_stream));
   });
   country_parser_.registerKeyword("mil_salaries", [this](std::istream& input_stream) {
      options_.mil_salary_level = parseBudgetLevel(commonItems::getString(input_stream));
   });
   country_parser_.registerKeyword("civil_war", [this](std::istream& input_stream) {
      options_.is_civil_war = commonItems::getString(input_stream) == "yes";
   });
   country_parser_.registerKeyword("cultures", [this](std::istream& input_stream) {
      for (const auto& culture_id: commonItems::getInts(input_stream))
      {
         options_.primary_culture_ids.emplace(culture_id);
      }
   });
   country_parser_.registerKeyword("ruler", [this](std::istream& input_stream) {
      const int64_t temp_number = commonItems::getLlong(input_stream);
      if (temp_number == 4294967295)
      {
         options_.head_of_state_id = -1;
      }
      else
      {
         options_.head_of_state_id = static_cast<int>(temp_number);
      }
   });

   country_parser_.registerKeyword("dead", [this](std::istream& input_stream) {
      options_.is_dead = commonItems::getString(input_stream) == "yes";
   });

   /// ---- counters parser ----

   counters_parser_.registerKeyword("legitimacy", [this](std::istream& input_stream) {
      options_.legitimacy = commonItems::getInt(input_stream);
   });

   counters_parser_.IgnoreUnregisteredItems();

   country_parser_.registerKeyword("counters", [this](std::istream& input_stream) {
      counters_parser_.parseStream(input_stream);
   });

   country_parser_.IgnoreUnregisteredItems();
}


std::optional<vic3::Country> vic3::CountryImporter::ImportCountry(const int number,
    std::istream& input_stream,
    const std::map<std::string, commonItems::Color>& color_definitions)
{
   options_ = {};
   country_parser_.parseStream(input_stream);

   if (options_.color == commonItems::Color())
   {
      if (const auto color_itr = color_definitions.find(options_.tag); color_itr != color_definitions.end())
      {
         options_.color = color_itr->second;
      }
   }

   options_.number = number;

   return Country(options_);
}