#ifndef HOI4_DECISIONS_H
#define HOI4_DECISIONS_H



#include "external/common_items/ConvenientParser.h"
#include "src/Configuration.h"
#include "src/HOI4World/Decisions/AgentRecruitmentDecisions.h"
#include "src/HOI4World/Decisions/DecisionsCategories.h"
#include "src/HOI4World/Decisions/DecisionsInCategory.h"
#include "src/HOI4World/Decisions/ExiledGovernmentsDecisions.h"
#include "src/HOI4World/Decisions/ForeignInfluenceDecisions.h"
#include "src/HOI4World/Decisions/GenericDecisions.h"
#include "src/HOI4World/Decisions/NavalTreatyDecisions.h"
#include "src/HOI4World/Decisions/PoliticalDecisions.h"
#include "src/HOI4World/Decisions/ResourceProspectingDecisions.h"
#include "src/HOI4World/Decisions/StabilityWarSupportDecisions.h"
#include "src/HOI4World/Events/Events.h"
#include "src/HOI4World/States/DefaultState.h"
#include <map>
#include <set>
#include <string>
#include <vector>



namespace HoI4
{

class decisions: commonItems::parser
{
  public:
	explicit decisions(const Configuration& theConfiguration);

	void updateDecisions(const std::set<std::string>& majorIdeologies,
		 const std::map<int, int>& provinceToStateIdMap,
		 const std::map<int, DefaultState>& defaultStates,
		 const Events& theEvents,
		 const std::set<std::string>& southAsianCountries,
		 const std::vector<std::string>& strongestGpNavies);

	[[nodiscard]] const auto& getIdeologicalCategories() const { return *ideologicalCategories; }
	[[nodiscard]] const std::vector<DecisionsInCategory>& getAgentRecruitmentDecisions() const
	{
		return agentRecruitmentDecisions.getDecisions();
	}
	[[nodiscard]] const std::vector<DecisionsInCategory>& getStabilityDecisions() const
	{
		return stabilityDecisions.getDecisions();
	}
	[[nodiscard]] const std::vector<DecisionsInCategory>& getPoliticalDecisions() const
	{
		return politicalDecisions.getDecisions();
	}
	[[nodiscard]] const std::vector<DecisionsInCategory>& getExiledGovernmentsDecisions() const
	{
		return exiledGovernmentsDecisions.getDecisions();
	}
	[[nodiscard]] const std::vector<DecisionsInCategory>& getForeignInfluenceDecisions() const
	{
		return foreignInfluenceDecisions.getDecisions();
	}
	[[nodiscard]] const std::vector<DecisionsInCategory>& getNavalTreatyDecisions() const
	{
		return navalTreatyDecisions.getDecisions();
	}
	[[nodiscard]] const std::vector<DecisionsInCategory>& getResourceProspectingDecisions() const
	{
		return resourceProspectingDecisions.getDecisions();
	}
	[[nodiscard]] const std::vector<DecisionsInCategory>& getGenericDecisions() const
	{
		return genericDecisions.getDecisions();
	}

  private:
	void generateIdeologicalCategories(const std::set<std::string>& majorIdeologies,
		 const std::map<int, int>& provinceToStateIdMap) const;
	static std::string getIdeologicalIcon(const std::string& ideology);
	static DecisionsCategory createLocalRecruitmentCategory(const std::map<int, int>& provinceToStateIdMap);
	static std::optional<int> getRelevantStateFromProvince(int province, const std::map<int, int>& provinceToStateIdMap);

	std::unique_ptr<DecisionsCategories> ideologicalCategories;

	AgentRecruitmentDecisions agentRecruitmentDecisions;
	StabilityWarSupportDecisions stabilityDecisions;
	PoliticalDecisions politicalDecisions;
	ExiledGovernmentsDecisions exiledGovernmentsDecisions;
	ForeignInfluenceDecisions foreignInfluenceDecisions;
	NavalTreatyDecisions navalTreatyDecisions;
	ResourceProspectingDecisions resourceProspectingDecisions;
	GenericDecisions genericDecisions;
};

} // namespace HoI4



#endif // HOI4_DECISIONS_H