#include "src/out_hoi4/countries/out_country.h"

#include <fstream>
#include <string>

#include "external/commonItems/OSCompatibilityLayer.h"
#include "external/fmt/include/fmt/format.h"
#include "src/out_hoi4/military/out_equipment_variant.h"
#include "src/out_hoi4/technology/out_technologies.h"

namespace
{
void OutputPolitics(std::ostream& country_history,
    const std::string& government_ideology,
    const date& last_election,
    const bool& elections_allowed,
    const std::map<std::string, int>& ideology_support)
{
   country_history << "set_politics = {\n";
   country_history << fmt::format("\truling_party = {}\n", government_ideology);
   country_history << fmt::format("\tlast_election = \"{}\"\n", last_election.toString());
   country_history << "\telection_frequency = 48\n";
   if (elections_allowed)
   {
      country_history << "\telections_allowed = yes\n";
   }
   else
   {
      country_history << "\telections_allowed = no\n";
   }
   country_history << "}\n";
   country_history << "\n";

   country_history << "set_popularities = {\n";
   for (const auto& [ideology, support]: ideology_support)
   {
      country_history << "\t" << ideology << " = " << support << "\n";
   }
   country_history << "}\n";
   country_history << "\n";
}
void OutputPuppets(std::ostream& output,
    const std::string& tag,
    const std::string& governmentIdeology,
    const std::set<std::string>& puppets)
{
   if (!puppets.empty())
   {
      output << "# DIPLOMACY\n";
      output << "if = {\n";
      output << "	limit = {\n";
      output << "		OR = {\n";
      output << "			has_dlc = \"Together for Victory\"\n";
      output << "			has_dlc = \"Man the Guns\"\n";
      output << "		}\n";
      output << "	}\n";
      for (const auto& puppet: puppets)
      {
         if (governmentIdeology == "fascism")
         {
            output << "    set_autonomy = {\n";
            output << "        target = " << puppet << "\n";
            output << "        autonomous_state = autonomy_integrated_puppet\n";
         }
         else
         {
            output << "    set_autonomy = {\n";
            output << "        target = " << puppet << "\n";
            output << "        autonomous_state = autonomy_puppet\n";
            output << "        freedom_level = 0.4\n";
         }
         output << "    }\n";
      }
      output << "    else = {\n";
      for (const auto& puppet: puppets)
      {
         if (governmentIdeology == "fascism")
         {
            output << "        set_autonomy = {\n";
            output << "            target = " << puppet << "\n";
            output << "            autonomous_state = autonomy_puppet\n";
            output << "        }\n";
         }
         else
         {
            output << "        puppet = " << puppet << "\n";
         }
      }
      output << "    }\n";
      output << "}\n";
      output << "\n";
   }
}
}  // namespace

void out::OutputCommonCountriesFile(std::string_view output_name, const hoi4::Country& country)
{
   const std::string& tag = country.GetTag();

   const auto common_country_file_name = fmt::format("output/{}/common/countries/{}.txt", output_name, tag);
   std::ofstream common_country(common_country_file_name);
   if (!common_country.is_open())
   {
      throw std::runtime_error(fmt::format("Could not create {}", common_country_file_name));
   }

   common_country << "graphical_culture = western_european_gfx\n";
   common_country << "graphical_culture_2d = western_european_2d\n";
   common_country << fmt::format("color {}", country.GetColor().outputRgb());

   common_country.close();
}


void out::OutputCommonCountryTag(const hoi4::Country& country, std::ofstream& tags_file)
{
   tags_file << fmt::format("{0} = \"countries/{0}.txt\"\n", country.GetTag());
}


void out::OutputCountryHistory(std::string_view output_name, const hoi4::Country& country)
{
   const std::string& tag = country.GetTag();

   const auto country_history_file_name = fmt::format("output/{}/history/countries/{}.txt", output_name, tag);
   std::ofstream country_history(country_history_file_name);
   if (!country_history.is_open())
   {
      throw std::runtime_error(fmt::format("Could not create {}", country_history_file_name));
   }

   if (const auto capital = country.GetCapitalState(); capital)
   {
      country_history << fmt::format("capital = {}\n", *capital);
   }
   country_history << "\n";
   country_history << fmt::format("oob = \"{}_1936\"", country.GetTag());
   country_history << "\n";
   country_history << "set_research_slots = 3\n";
   country_history << "set_convoys = 0\n";
   country_history << "\n";

   OutputPolitics(country_history,
       country.GetIdeology(),
       country.GetLastElection(),
       country.AreElectionsAllowed(),
       country.GetIdeologySupport());

   OutputPuppets(country_history, country.GetTag(), country.GetIdeology(), country.GetPuppets());

   country_history << "add_ideas = {\n";
   country_history << "\tlimited_conscription\n";
   country_history << "\tcivilian_economy\n";
   country_history << "\texport_focus\n";
   for (const std::string& idea: country.GetIdeas())
   {
      country_history << fmt::format("\t{}\n", idea);
   }
   country_history << "}\n";
   country_history << "set_stability = 0.60\n";
   country_history << "set_war_support = 0.60\n";
   country_history << "\n";

   country_history << country.GetTechnologies();

   country_history << "if = {\n";
   country_history << "\tlimit = { not = { has_dlc = \"Man the Guns\" } }\n";
   for (const auto& variant: country.GetLegacyShipVariants())
   {
      country_history << variant;
   }
   country_history << "}\n";
   country_history << "if = {\n";
   country_history << "\tlimit = { has_dlc = \"Man the Guns\" }\n";
   for (const auto& variant: country.GetShipVariants())
   {
      country_history << variant;
   }
   country_history << "}\n";
   country_history << "if = {\n";
   country_history << "\tlimit = { has_dlc = \"By Blood Alone\" }\n";
   for (const auto& variant: country.GetPlaneVariants())
   {
      country_history << variant;
   }
   country_history << "}\n";
   country_history << "if = {\n";
   country_history << "\tlimit = { has_dlc = \"No Step Back\" }\n";
   for (const auto& variant: country.GetTankVariants())
   {
      country_history << variant;
   }
   country_history << "}\n";

   country_history.close();
}