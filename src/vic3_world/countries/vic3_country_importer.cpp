#include "src/vic3_world/countries/vic3_country_importer.h"

#include "external/commonItems/CommonRegexes.h"
#include "external/commonItems/ParserHelpers.h"
#include "external/commonItems/StringUtils.h"



vic3::CountryImporter::CountryImporter()
{
   country_parser_.registerKeyword("definition", [this](std::istream& input_stream) {
      tag_ = commonItems::remQuotes(commonItems::getString(input_stream));
   });
   country_parser_.registerKeyword("capital", [this](std::istream& input_stream) {
      capital_ = commonItems::getInt(input_stream);
   });
   country_parser_.registerKeyword("country_type", [this](std::istream& input_stream) {
      country_type_ = commonItems::getString(input_stream);
   });
   country_parser_.registerKeyword("civil_war", [this](std::istream& input_stream) {
      is_civil_war_ = commonItems::getString(input_stream) == "yes";
   });
   country_parser_.registerKeyword("cultures", [this](std::istream& input_stream) {
      for (const auto& culture_id: commonItems::getInts(input_stream))
      {
         primary_culture_ids_.emplace(culture_id);
      }
   });
   country_parser_.registerKeyword("ruler", [this](std::istream& input_stream) {
      head_of_state_id_ = commonItems::getInt(input_stream);
   });

   country_parser_.registerKeyword("dead", [this](std::istream& input_stream) {
      is_dead_ = commonItems::getString(input_stream) == "yes";
   });

   /// ---- counters parser ----

   counters_parser_.registerKeyword("legitimacy", [this](std::istream& input_stream) {
      legitimacy_ = commonItems::getInt(input_stream);
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
   tag_.clear();
   capital_ = std::nullopt;
   country_type_.clear();
   primary_culture_ids_.clear();
   head_of_state_id_ = 0;
   is_dead_ = false;

   country_parser_.parseStream(input_stream);
   if (is_dead_)
   {
      return std::nullopt;
   }

   commonItems::Color color;
   if (const auto color_itr = color_definitions.find(tag_); color_itr != color_definitions.end())
   {
      color = color_itr->second;
   }

   return Country({.number = number,
       .tag = tag_,
       .color = color,
       .capital_state = capital_,
       .country_type = country_type_,
       .is_civil_war = is_civil_war_,
       .primary_culture_ids = primary_culture_ids_,
       .head_of_state_id = head_of_state_id_,
       .legitimacy = legitimacy_});
}