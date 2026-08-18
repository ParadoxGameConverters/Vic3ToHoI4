#include "out_world.h"

#include <external/fmt/include/fmt/format.h>
#include <external/fmt/include/fmt/ranges.h>

#include <cctype>
#include <fstream>
#include <ranges>

#include "src/out_hoi4/countries/out_countries.h"
#include "src/out_hoi4/countries/out_tag_aliases.h"
#include "src/out_hoi4/decisions/out_decisions.h"
#include "src/out_hoi4/decisions/out_decisions_categories.h"
#include "src/out_hoi4/events/out_events.h"
#include "src/out_hoi4/ideas/out_ideas.h"
#include "src/out_hoi4/localizations/out_localizations.h"
#include "src/out_hoi4/map/out_buildings.h"
#include "src/out_hoi4/map/out_railways.h"
#include "src/out_hoi4/map/out_strategic_regions.h"
#include "src/out_hoi4/map/out_supply_nodes.h"
#include "src/out_hoi4/names/out_names.h"
#include "src/out_hoi4/portraits/out_portraits.h"
#include "src/out_hoi4/states/out_states.h"
#include "src/support/date_fmt.h"



using std::filesystem::path;



namespace
{

void OutputScriptedEffects(const path& output_name, const std::vector<std::string>& scripted_effects)
{
   const auto scripted_effects_path = "output" / output_name / "common/scripted_effects";
   if (!commonItems::DoesFolderExist(scripted_effects_path) &&
       !std::filesystem::create_directories(scripted_effects_path))
   {
      throw std::runtime_error("Could not create " + scripted_effects_path.string());
   }

   const path scripted_effects_filename =
       "output" / output_name / "common/scripted_effects/converter_scripted_effects.txt";
   std::ofstream scripted_effects_file(scripted_effects_filename);
   if (!scripted_effects_file.is_open())
   {
      throw std::runtime_error(fmt::format("Could not create {}", scripted_effects_filename.string()));
   }

   const std::string output_string = fmt::format("{}", fmt::join(scripted_effects, "\n\n"));
   scripted_effects_file << output_string;

   scripted_effects_file.close();
}


struct Powers
{
   std::set<std::string> great_powers;
   std::set<std::string> major_powers;
};


void OutputBookmark(const path& output_name,
    std::string_view bookmark_name,
    date start_date,
    bool default_bookmark,
    const Powers& powers)
{
   std::string uppercase_bookmark_name{bookmark_name};
#pragma warning(push)
#pragma warning(disable : 4242)
   std::ranges::transform(uppercase_bookmark_name, uppercase_bookmark_name.begin(), ::toupper);
#pragma warning(pop)

   const path bookmark_file_name = "output" / output_name / fmt::format("common/bookmarks/the_{}.txt", bookmark_name);
   std::ofstream bookmark_file(bookmark_file_name);
   if (!bookmark_file.is_open())
   {
      throw std::runtime_error(fmt::format("Could not create {}", bookmark_file_name.string()));
   }

   bookmark_file << "bookmarks = {\n";
   bookmark_file << "\tbookmark = {\n";
   bookmark_file << fmt::format("\t\tname = {}_NAME\n", uppercase_bookmark_name);
   bookmark_file << fmt::format("\t\tdesc = {}_DESC\n", uppercase_bookmark_name);
   bookmark_file << fmt::format("\t\tdate = {}.12\n", start_date);
   bookmark_file << fmt::format("\t\tpicture = GFX_select_date_193{}\n", start_date == date(1936, 1, 1) ? '6' : '9');
   bookmark_file << "\t\tdefault_country = \"---\"\n";
   if (default_bookmark)
   {
      bookmark_file << "\t\tdefault = yes\n";
   }

   for (const std::string& great_power: powers.great_powers)
   {
      bookmark_file << fmt::format("\t\t{}= {{}}\n", great_power);
   }

   bookmark_file << "\t\t\"---\"= {\n";
   bookmark_file << fmt::format("\t\t\thistory = \"OTHER_{}_DESC\"\n", uppercase_bookmark_name);
   bookmark_file << "\t\t}\n";

   for (const auto& major_power: powers.major_powers)
   {
      bookmark_file << fmt::format("\t\t{} = {{\n", major_power);
      bookmark_file << "\t\t\tminor = yes\n";
      bookmark_file << "\t\t}\n";
   }

   bookmark_file << "\t\teffect = {\n";
   bookmark_file << "\t\t\trandomize_weather = 22345 # <-Obligatory in every bookmark !\n";
   bookmark_file << "\t\t\t#123 = { rain_light = yes }\n";
   bookmark_file << "\t\t}\n";
   bookmark_file << "\t}\n";
   bookmark_file << "}\n";
   bookmark_file.close();
}


void OutputCultureArrays(const path& output_name,
    const std::map<std::string, std::set<int>>& homelands,
    const std::map<std::string, std::set<std::string>>& primary_culture_countries)
{
   const path cultures_filename = "output" / output_name / "common/on_actions/converter_cultures.txt";
   std::ofstream cultures_file(cultures_filename);
   if (!cultures_file.is_open())
   {
      throw std::runtime_error(fmt::format("Could not create {}", cultures_filename.string()));
   }

   std::println(cultures_file, "on_actions = {{");
   std::println(cultures_file, "\ton_startup = {{");
   std::println(cultures_file, "\t\teffect = {{");
   for (const auto& [culture, states]: homelands)
   {
      for (const int state: states)
      {
         std::println(cultures_file, "\t\t\tadd_to_array = {{ global.{}_states_array = {} }}", culture, state);
      }
   }
   for (const auto& [culture, countries]: primary_culture_countries)
   {
      for (const std::string& country: countries)
      {
         std::println(cultures_file, "\t\t\tadd_to_array = {{ global.{}_countries_array = {} }}", culture, country);
      }
   }
   std::println(cultures_file, "\t\t}}");
   std::println(cultures_file, "\t}}");
   std::println(cultures_file, "}}");
}

}  // namespace


namespace out
{

void OutputWorld(const path& output_name, const hoi4::World& world)
{
   OutputCountries(output_name, world.GetCountries(), world.GetCharacters());
   OutputTagAliases(output_name, world.GetTagAliases());
   OutputMonarchIdeas(output_name, world.GetCountries(), world.GetCharacters());
   OutputStates(output_name, world.GetStates().states);
   OutputStrategicRegions(output_name, world.GetStrategicRegions());
   OutputBuildings(output_name, world.GetBuildings());
   OutputRailways(output_name, world.GetRailways().railways);
   OutputSupplyNodes(output_name, world.GetRailways().supply_nodes);
   OutputLocalizations(output_name, world.GetLocalizations());
   OutputNames(output_name, world.GetCountries());
   OutputPortraits(output_name, world.GetCountries());
   OutputDecisionsCategories(output_name, world.GetDecisionsCategories());
   OutputDecisions(output_name, world.GetDecisionsInCategories());
   OutputEvents(output_name, world.GetEvents());
   OutputScriptedEffects(output_name, world.GetScriptedEffects());
   OutputBookmark(output_name,
       "grand_campaign",
       date("1936.1.1"),
       true,
       {
           .great_powers = world.GetGreatPowers(),
           .major_powers = world.GetMajorPowers(),
       });
   OutputCultureArrays(output_name, world.GetHomelands(), world.GetPrimaryCultureCountries());
}

}  // namespace out