#include "src/vic3_world/countries/vic3_country_importer.h"

#include "external/commonItems/CommonRegexes.h"
#include "external/commonItems/ParserHelpers.h"
#include "external/commonItems/StringUtils.h"
#include "external/fmt/include/fmt/format.h"


vic3::BudgetLevel parseBudgetLevel(const std::string& level_string)
{
   if (level_string == "very_low")
   {
      return vic3::BudgetLevel::VeryLow;
   }
   else if (level_string == "low")
   {
      return vic3::BudgetLevel::Low;
   }
   else if (level_string == "medium")
   {
      return vic3::BudgetLevel::Medium;
   }
   else if (level_string == "high")
   {
      return vic3::BudgetLevel::High;
   }
   else if (level_string == "very_high")
   {
      return vic3::BudgetLevel::VeryHigh;
   }
   else
   {
      Log(LogLevel::Error) << fmt::format("Unknown budget level {}", level_string);
      return vic3::BudgetLevel::Medium;
   }
}


vic3::CountryImporter::CountryImporter(): options_()
{
   country_parser_.registerKeyword("definition", [this](std::istream& input_stream) {
      options_.tag = commonItems::remQuotes(commonItems::getString(input_stream));
   });
   country_parser_.registerKeyword("capital", [this](std::istream& input_stream) {
      options_.capital_state = commonItems::getInt(input_stream);
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
      options_.head_of_state_id = commonItems::getInt(input_stream);
   });

   country_parser_.registerKeyword("dead", [this](std::istream& input_stream) {
      is_dead_ = commonItems::getString(input_stream) == "yes";
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
   options_.tag.clear();
   options_.capital_state = std::nullopt;
   options_.country_type.clear();
   options_.primary_culture_ids.clear();
   options_.head_of_state_id = 0;
   is_dead_ = false;

   country_parser_.parseStream(input_stream);
   if (is_dead_)
   {
      return std::nullopt;
   }

   commonItems::Color color;
   if (const auto color_itr = color_definitions.find(options_.tag); color_itr != color_definitions.end())
   {
      options_.color = color_itr->second;
   }

   options_.number = number;

   return Country(options_);
}