#include "src/out_hoi4/countries/out_country.h"

#include <external/commonItems/OSCompatibilityLayer.h>
#include <external/fmt/include/fmt/format.h>

#include <fstream>
#include <ios>
#include <string>

#include "src/out_hoi4/characters/out_character.h"
#include "src/out_hoi4/diplomacy/out_war.h"
#include "src/out_hoi4/military/out_equipment_variant.h"
#include "src/out_hoi4/technology/out_technologies.h"



namespace
{

constexpr std::string_view kShip =
    "\t\t\tship = {{"
    " name = \"{}\""
    " definition = {}"
    " equipment = {{ {} = {{ "
    " amount = 1 "
    " owner = {} "
    " version_name = \"{}\" }} }} }}\n";
constexpr std::string_view kNavyHeader =
    "\tfleet = {{\n"
    "\t\tname = \"{0}\"\n"
    "\t\tnaval_base = {1}\n"
    "\t\ttask_force = {{\n"
    "\t\t\tname = \"{0}\"\n"
    "\t\t\tlocation = {1}\n";
constexpr std::string_view kNavyFooter =
    "\t\t}\n"
    "\t}\n";


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


void OutputCharacterRecruitment(std::ostream& country_history,
    const std::string& tag,
    const std::vector<int>& leader_ids)
{
   for (const auto leader_id: leader_ids)
   {
      country_history << fmt::format("recruit_character = {}_{}\n", tag, leader_id);
   }
}


void OutputSpies(std::ostream& country_history,
    const std::set<int>& spy_ids,
    const std::map<int, hoi4::Character>& characters)
{
   country_history << "if = {\n";
   country_history << "\tlimit = { has_dlc = \"La Resistance\" }\n";
   for (const auto spy_id: spy_ids)
   {
      out::OutputSpy(country_history, characters.at(spy_id));
   }
   country_history << "}\n";
   country_history << "\n";
}


void OutputOverlord(std::ostream& output,
    std::string_view self_tag,
    std::string_view overlord_tag,
    const std::string& government_ideology)
{
   output << fmt::format("{} = {{\n", overlord_tag);
   output << "\tif = {\n";
   output << "\t\tlimit = {\n";
   output << "\t\t\thas_dlc = \"Together for Victory\"\n";
   output << "\t\t}\n";
   output << "\t\tset_autonomy = {\n";
   output << fmt::format("\t\t\ttarget = {}\n", self_tag);
   if (government_ideology == "fascism")
   {
      output << "\t\t\tautonomous_state = autonomy_integrated_puppet\n";
   }
   else
   {
      output << "\t\t\tautonomous_state = autonomy_puppet\n";
      output << "\t\t\tfreedom_level = 0.4\n";
   }
   output << "\t\t}\n";
   output << "\t}\n";
   output << "\telse = {\n";
   if (government_ideology == "fascism")
   {
      output << "\t\tset_autonomy = {\n";
      output << fmt::format("\t\t\ttarget = {}\n", self_tag);
      output << "\t\t\tautonomous_state = autonomy_puppet\n";
      output << "\t\t}\n";
   }
   else
   {
      output << fmt::format("\t\tpuppet = {}\n", self_tag);
   }
   output << "\t}\n";
   output << "}\n";
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


void out::OutputCommonCharactersFile(std::string_view output_name,
    const hoi4::Country& country,
    const std::map<int, hoi4::Character>& characters)
{
   const std::string& tag = country.GetTag();

   const auto common_characters_file_name = fmt::format("output/{}/common/characters/{}.txt", output_name, tag);
   std::ofstream common_characters(common_characters_file_name);
   if (!common_characters.is_open())
   {
      throw std::runtime_error(fmt::format("Could not create {}", common_characters_file_name));
   }

   common_characters << "characters = {\n";
   for (const auto leader_id: country.GetLeaderIds())
   {
      OutputCharacter(common_characters, country.GetTag(), characters.at(leader_id));
   }
   common_characters << "}\n";
}


void out::OutputCountryHistory(std::string_view output_name,
    const hoi4::Country& country,
    const std::map<int, hoi4::Character>& characters)
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
   country_history << fmt::format("set_research_slots = {}\n", country.GetStartingResearchSlots());
   for (const auto& war: country.GetWars())
   {
      country_history << war;
   }
   country_history << fmt::format("set_convoys = {}\n", country.GetConvoys());
   country_history << "\n";

   OutputCharacterRecruitment(country_history, tag, country.GetLeaderIds());
   OutputSpies(country_history, country.GetSpyIds(), characters);

   if (const auto& overlord = country.GetOverlord(); overlord != std::nullopt)
   {
      OutputOverlord(country_history, tag, *overlord, country.GetIdeology());
   }

   OutputPolitics(country_history,
       country.GetIdeology(),
       country.GetLastElection(),
       country.AreElectionsAllowed(),
       country.GetIdeologySupport());

   country_history << "add_ideas = {\n";
   for (const std::string& idea: country.GetIdeas())
   {
      country_history << fmt::format("\t{}\n", idea);
   }
   for (const std::string& law: country.GetLaws())
   {
      country_history << fmt::format("\t{}\n", law);
   }
   country_history << "}\n";
   country_history << fmt::format("set_stability = {}\n", country.GetStability());
   country_history << fmt::format("set_war_support = {:.2f}\n", country.GetWarSupport());
   country_history << "\n";

   country_history << country.GetTechnologies();

   country_history << "if = {\n";
   country_history << "\tlimit = { not = { has_dlc = \"Man the Guns\" } }\n";
   country_history << fmt::format("\tset_naval_oob = {}_1936_Naval_Legacy\n", tag);
   for (const auto& variant: country.GetLegacyShipVariants())
   {
      country_history << variant;
   }
   country_history << "}\n";
   country_history << "if = {\n";
   country_history << "\tlimit = { has_dlc = \"Man the Guns\" }\n";
   country_history << fmt::format("\tset_naval_oob = {}_1936_Naval\n", tag);
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

void outputShip(std::ofstream& navy, std::ofstream& lgcy, const std::string& tag, const hoi4::Ship& ship)
{
   navy << fmt::format(kShip, ship.GetName(), ship.GetDefinition(), ship.GetEquipment(), tag, ship.GetVersion());
   lgcy << fmt::format(kShip, ship.GetName(), ship.GetDefinition(), ship.GetLegacyEquipment(), tag, ship.GetVersion());
}


void outputNavy(std::ofstream& navy,
    std::ofstream& legacy,
    const std::string& tag,
    const std::vector<hoi4::TaskForce>& task_forces)
{
   if (task_forces.empty())
   {
      return;
   }
   navy << "units = {\n";
   legacy << "units = {\n";

   for (const auto& task_force: task_forces)
   {
      const std::string header = fmt::format(kNavyHeader, task_force.name, task_force.location);
      navy << header;
      legacy << header;
      for (const auto& ship: task_force.ships)
      {
         outputShip(navy, legacy, tag, ship);
      }
      navy << kNavyFooter;
      legacy << kNavyFooter;
   }
   navy << "}\n";
   legacy << "}\n";
}

void out::OutputCountryNavy(std::string_view output_name, const hoi4::Country& country)
{
   const std::string& tag = country.GetTag();
   const auto naval_file_name = fmt::format("output/{}/history/units/{}_1936_Naval.txt", output_name, tag);
   std::ofstream navy(naval_file_name);
   if (!navy.is_open())
   {
      throw std::runtime_error(fmt::format("Could not create {}", naval_file_name));
   }
   const auto legacy_file_name = fmt::format("output/{}/history/units/{}_1936_Naval_Legacy.txt", output_name, tag);
   std::ofstream legacy(legacy_file_name);
   if (!legacy.is_open())
   {
      throw std::runtime_error(fmt::format("Could not create {}", legacy_file_name));
   }

   navy << "# Naval OOB\n";
   legacy << "# Naval non-MTG OOB\n";
   outputNavy(navy, legacy, tag, country.GetTaskForces());
   navy.close();
   legacy.close();
}

void out::OutputCountryUnits(const std::string& oob_file, const hoi4::Country& country)
{
   if (country.GetUnits().empty())
   {
      return;
   }
   // Note append mode as division templates were copied, not output.
   std::ofstream country_oob(oob_file, std::ios_base::app);
   if (!country_oob.is_open())
   {
      throw std::runtime_error(fmt::format("Could not open {} for updates", oob_file));
   }
   country_oob << "\n\nunits = {\n";
   std::map<std::string, int> counts;
   for (const auto& unit: country.GetUnits())
   {
      counts[unit.unit_template]++;
      country_oob << "\tdivision = {\n";
      country_oob << "\t\tdivision_name = {\n";
      country_oob << "\t\t\tis_name_ordered = yes\n";
      country_oob << "\t\t\tname_order = " << counts[unit.unit_template] << "\n";
      country_oob << "\t\t}\n";
      country_oob << "\t\tlocation = " << unit.location << "\n";
      country_oob << "\t\tdivision_template = \"" << unit.unit_template << "\"\n";
      country_oob << "\t\tstart_equipment_factor = " << 0.01 * unit.equipment << "\n";
      country_oob << "\t\tstart_experience_factor = 0.2\n";
      country_oob << "\t}\n";
   }
   country_oob << "}\n";
   country_oob.close();
}
