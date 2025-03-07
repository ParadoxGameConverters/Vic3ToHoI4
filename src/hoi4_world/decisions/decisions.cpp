#include "src/HOI4World/Decisions/Decisions.h"

#include <regex>

#include "external/common_items/Log.h"
#include "src/HOI4World/Events/Events.h"



HoI4::decisions::decisions(const Configuration& theConfiguration)
{
   ideologicalCategories = DecisionsCategories::Factory().getDecisionsCategories(
       "blankmod/common/decisions/categories/00_decision_categories.txt");
   agentRecruitmentDecisions.importDecisions(
       theConfiguration.getHoI4Path() / "common/decisions/lar_agent_recruitment_decisions.txt");
   stabilityDecisions.importDecisions(theConfiguration.getHoI4Path() / "common/decisions/stability_war_support.txt");
   politicalDecisions.importDecisions("Configurables/ideologicalDecisions.txt");
   exiledGovernmentsDecisions.importDecisions("blankmod/common/decisions/_exiled_governments_decisions.txt");
   foreignInfluenceDecisions.importDecisions(theConfiguration.getHoI4Path() / "common/decisions/foreign_influence.txt");
   foreignInfluenceDecisions.importDecisions("Configurables/foreignInfluenceDecisions.txt");
   navalTreatyDecisions.importDecisions(theConfiguration.getHoI4Path() / "common/decisions/MTG_naval_treaty.txt");
   resourceProspectingDecisions.importDecisions("blankmod/common/decisions/resource_prospecting.txt");
   genericDecisions.importDecisions("blankmod/common/decisions/_generic_decisions.txt");
}


void HoI4::decisions::updateDecisions(const std::set<std::string>& majorIdeologies,
    const std::map<int, int>& provinceToStateIdMap,
    const std::map<int, DefaultState>& defaultStates,
    const Events& theEvents,
    const std::set<std::string>& southAsianCountries,
    const std::vector<std::string>& strongestGpNavies)
{
   Log(LogLevel::Info) << "\tUpdating decisions";

   generateIdeologicalCategories(majorIdeologies, provinceToStateIdMap);

   agentRecruitmentDecisions.updateDecisions(southAsianCountries);
   stabilityDecisions.updateDecisions(majorIdeologies);
   politicalDecisions.updateDecisions(majorIdeologies, theEvents);
   exiledGovernmentsDecisions.updateDecisions(majorIdeologies);
   foreignInfluenceDecisions.updateDecisions(majorIdeologies);
   navalTreatyDecisions.updateDecisions(majorIdeologies, strongestGpNavies);
   resourceProspectingDecisions.updateDecisions(provinceToStateIdMap, defaultStates);
   genericDecisions.updateDecisions(provinceToStateIdMap, majorIdeologies);
}



void HoI4::decisions::generateIdeologicalCategories(const std::set<std::string>& majorIdeologies,
    const std::map<int, int>& provinceToStateIdMap) const
{
   DecisionsCategory::Factory decisionsCategoryFactory;

   for (const auto& majorIdeology: majorIdeologies)
   {
      if (majorIdeology == "neutrality")
      {
         continue;
      }

      std::stringstream input;
      input << "= {\n";
      input << "\ticon = " << getIdeologicalIcon(majorIdeology) << "\n";
      input << "\n";
      input << "\tallowed = {\n";
      input << "\t\talways = yes\n";
      input << "\t}\n";
      input << "\n";
      input << "\t#visible = {\n";
      input << "\t#	NOT = { has_government = " << majorIdeology << " }\n";
      input << "\t#	has_idea_with_trait = " << majorIdeology << "_minister\n";
      input << "\t#}\n";
      input << "}\n";
      input << "\n";

      ideologicalCategories->addCategory(
          decisionsCategoryFactory.getDecisionsCategory(majorIdeology + "_on_the_rise", input));
   }

   ideologicalCategories->replaceCategory(createLocalRecruitmentCategory(provinceToStateIdMap));
}


std::string HoI4::decisions::getIdeologicalIcon(const std::string& ideology)
{
   if ((ideology == "fascism") || (ideology == "absolutist"))
   {
      return "generic_fascism";
   }
   if ((ideology == "communism") || (ideology == "radical"))
   {
      return "generic_communism";
   }

   return "generic_democracy";
}


HoI4::DecisionsCategory HoI4::decisions::createLocalRecruitmentCategory(const std::map<int, int>& provinceToStateIdMap)
{
   std::stringstream input;
   input << "= {\n";
   input << "\tallowed = { has_dlc = \"La Resistance\" }\n";
   input << "\tvisible = { has_done_agency_upgrade = upgrade_training_centers }\n";
   input << "\tvisibility_type = map_and_decisions_view\n";

   auto europeanState = getRelevantStateFromProvince(6583, provinceToStateIdMap);
   if (europeanState)
   {
      input << "\ton_map_area = {\n";
      input << "\t\tstate = " << *europeanState << "\n";
      input << "\t\tname = LAR_recruitment_europe\n";
      input << "\t\tzoom = 650\n";
      input << "\n";
      input << "\t\ttarget_root_trigger = {\n";
      input << "\t\t\tOR =	{\n";
      input << "\t\t\t\thas_country_flag = europe_recruitment_unlocked\n";
      input << "\t\t\t\tcapital_scope = { is_on_continent = europe }\n";
      input << "\t\t\t}\n";
      input << "\t\t}\n";
      input << "\t}\n";
   }

   auto northAmericanState = getRelevantStateFromProvince(10717, provinceToStateIdMap);
   if (northAmericanState)
   {
      input << "\ton_map_area = {\n";
      input << "\t\tstate = " << *northAmericanState << "\n";
      input << "\t\tname = LAR_recruitment_north_america\n";
      input << "\t\tzoom = 850\n";
      input << "\n";
      input << "\t\ttarget_root_trigger = {\n";
      input << "\t\t\tOR =	{\n";
      input << "\t\t\t\thas_country_flag = north_america_recruitment_unlocked\n";
      input << "\t\t\t\tcapital_scope = { is_on_continent = north_america }\n";
      input << "\t\t\t}\n";
      input << "\t\t}\n";
      input << "\t}\n";
   }

   auto southAmericanState = getRelevantStateFromProvince(10924, provinceToStateIdMap);
   if (southAmericanState)
   {
      input << "\ton_map_area = {\n";
      input << "\t\tstate = " << *southAmericanState << "\n";
      input << "\t\tname = LAR_recruitment_south_america\n";
      input << "\t\tzoom = 850\n";
      input << "\n";
      input << "\t\ttarget_root_trigger = {\n";
      input << "\t\t\tOR =	{\n";
      input << "\t\t\t\thas_country_flag = south_america_recruitment_unlocked\n";
      input << "\t\t\t\tcapital_scope = { is_on_continent = south_america }\n";
      input << "\t\t\t}\n";
      input << "\t\t}\n";
      input << "\t}\n";
   }

   auto africanState = getRelevantStateFromProvince(5117, provinceToStateIdMap);
   if (africanState)
   {
      input << "\ton_map_area = {\n";
      input << "\t\tstate = " << *africanState << "\n";
      input << "\t\tname = LAR_recruitment_africa\n";
      input << "\t\tzoom = 850\n";
      input << "\n";
      input << "\t\ttarget_root_trigger = {\n";
      input << "\t\t\tOR =	{\n";
      input << "\t\t\t\thas_country_flag = africa_recruitment_unlocked\n";
      input << "\t\t\t\tcapital_scope = { is_on_continent = africa }\n";
      input << "\t\t\t}\n";
      input << "\t\t}\n";
      input << "\t}\n";
   }

   auto asianState = getRelevantStateFromProvince(9843, provinceToStateIdMap);
   if (asianState)
   {
      input << "\ton_map_area = {\n";
      input << "\t\tstate = " << *asianState << "\n";
      input << "\t\tname = LAR_recruitment_asia\n";
      input << "\t\tzoom = 850\n";
      input << "\n";
      input << "\t\ttarget_root_trigger = {\n";
      input << "\t\t\tOR = {\n";
      input << "\t\t\t\thas_country_flag = asia_recruitment_unlocked\n";
      input << "\t\t\t\tcapital_scope = { is_on_continent = asia }\n";
      input << "\t\t\t}\n";
      input << "\t\t\tNOT = {\n";
      input << "\t\t\t\thas_country_flag = conv_south_asia\n";
      input << "\t\t\t}\n";
      input << "\t\t}\n";
      input << "\t}\n";
   }

   auto australianState = getRelevantStateFromProvince(4864, provinceToStateIdMap);
   if (australianState)
   {
      input << "\ton_map_area = {\n";
      input << "\t\tstate = " << *australianState << "\n";
      input << "\t\tname = LAR_recruitment_oceania\n";
      input << "\t\tzoom = 850\n";
      input << "\n";
      input << "\t\ttarget_root_trigger = {\n";
      input << "\t\t\tOR =	{\n";
      input << "\t\t\t\thas_country_flag = australia_recruitment_unlocked\n";
      input << "\t\t\t\tcapital_scope = { is_on_continent = australia }\n";
      input << "\t\t\t}\n";
      input << "\t\t}\n";
      input << "\t}\n";
   }

   auto southAsianState = getRelevantStateFromProvince(2086, provinceToStateIdMap);
   if (southAsianState)
   {
      input << "\ton_map_area = {\n";
      input << "\t\tstate = " << *southAsianState << "\n";
      input << "\t\tname = LAR_recruitment_india\n";
      input << "\t\tzoom = 850\n";
      input << "\n";
      input << "\t\ttarget_root_trigger = {\n";
      input << "\t\t\tOR = {\n";
      input << "\t\t\t\thas_country_flag = india_recruitment_unlocked\n";
      input << "\t\t\t\tcapital_scope = { is_on_continent = asia }\n";
      input << "\t\t\t}\n";
      input << "\t\t\thas_country_flag = conv_south_asia\n";
      input << "\t\t}\n";
      input << "\t}\n";
   }

   auto middleEasternState = getRelevantStateFromProvince(8085, provinceToStateIdMap);
   if (middleEasternState)
   {
      input << "\ton_map_area = {\n";
      input << "\t\tstate = " << *middleEasternState << "\n";
      input << "\t\tname = LAR_recruitment_middle_east\n";
      input << "\t\tzoom = 850\n";
      input << "\n";
      input << "\t\ttarget_root_trigger = {\n";
      input << "\t\t\tOR =	{\n";
      input << "\t\t\t\thas_country_flag = middle_east_recruitment_unlocked\n";
      input << "\t\t\t\tcapital_scope = { is_on_continent = middle_east }\n";
      input << "\t\t\t}\n";
      input << "\t\t}\n";
      input << "\t}\n";
   }

   return DecisionsCategory::Factory().getDecisionsCategory("lar_local_recruitment", input);
}


std::optional<int> HoI4::decisions::getRelevantStateFromProvince(int province,
    const std::map<int, int>& provinceToStateIdMap)
{
   if (const auto& mapping = provinceToStateIdMap.find(province); mapping != provinceToStateIdMap.end())
   {
      return mapping->second;
   }

   return std::nullopt;
}