#include "src/vic3_world/states/vic3_state_importer.h"

#include "external/commonItems/CommonRegexes.h"
#include "external/commonItems/ParserHelpers.h"



vic3::StateImporter::StateImporter()
{
   state_parser_.registerKeyword("country", [this](std::istream& input_stream) {
      owner_number_ = commonItems::getInt(input_stream);
   });
   state_parser_.registerKeyword("provinces", [this](std::istream& input_stream) {
      provinces_parser_.parseStream(input_stream);
   });
   state_parser_.registerKeyword("pop_statistics", [this](std::istream& input_stream) {
      pop_statistics_parser_.parseStream(input_stream);
   });
   state_parser_.IgnoreUnregisteredItems();

   provinces_parser_.registerKeyword("provinces", [this](std::istream& input_stream) {
      const auto provinces_input = commonItems::getInts(input_stream);
      if ((provinces_input.size() % 2) != 0)
      {
         throw std::runtime_error(
             "Provinces entry was malformed. Please go to the forum thread and upload log.txt and your save.");
      }
      for (unsigned int i = 0; i < provinces_input.size(); i += 2U)
      {
         const int start_province = provinces_input[i];
         const int additional_provinces = provinces_input[i + 1U];
         for (int j = 0; j <= additional_provinces; j++)
         {
            provinces_.insert(start_province + j);
         }
      }
   });

   pop_statistics_parser_.registerKeyword("lower_strata_pops", [this](std::istream& input_stream) {
      population_ += commonItems::getInt(input_stream);
   });
   pop_statistics_parser_.registerKeyword("middle_strata_pops", [this](std::istream& input_stream) {
      population_ += commonItems::getInt(input_stream);
   });
   pop_statistics_parser_.registerKeyword("upper_strata_pops", [this](std::istream& input_stream) {
      population_ += commonItems::getInt(input_stream);
   });
   pop_statistics_parser_.registerKeyword("salaried_working_adults", [this](std::istream& input_stream) {
      employed_population_ += commonItems::getInt(input_stream);
   });
   pop_statistics_parser_.registerKeyword("unemployed_working_adults", [this](std::istream& input_stream) {
      employed_population_ += commonItems::getInt(input_stream);
   });
   pop_statistics_parser_.registerKeyword("laborer_working_adults", [this](std::istream& input_stream) {
      employed_population_ += commonItems::getInt(input_stream);
   });
   pop_statistics_parser_.IgnoreUnregisteredItems();
}


vic3::State vic3::StateImporter::ImportState(std::istream& input_stream)
{
   owner_number_.reset();
   provinces_.clear();
   population_ = 0;
   employed_population_ = 0;

   state_parser_.parseStream(input_stream);

   return State({.owner_number = owner_number_,
       .provinces = provinces_,
       .population = population_,
       .employed_population = employed_population_});
}