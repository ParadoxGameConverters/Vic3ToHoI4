#include "src/hoi4_world/countries/hoi4_country.h"

#include <sstream>

#include "external/fmt/include/fmt/format.h"

namespace hoi4
{


void PrintTo(const Country& country, std::ostream* os)
{
   *os << "\nTag          = " << country.tag_ << "\n"
       << "Capital        = " << country.capital_state_.value_or(-1) << "\n"
       << "Ideology       = " << country.ideology_ << " (" << country.sub_ideology_ << ")\n"
       << "Elections      = " << country.last_election_ << " (" << country.has_elections_ << ")\n"
       << "Economy        = " << country.economy_law_ << "\n"
       << "Trade          = " << country.trade_law_ << "\n"
       << "Military       = " << country.military_law_ << "\n"
       << "Monarch idea   = " << country.monarch_idea_id_.value_or(-1) << "\n"
       << "Overlord       = " << country.overlord_.value_or("none") << "\n"
       << "Research slots = " << country.starting_research_slots_ << "\n"
       << "Stability      = " << country.stability_ << "\n"
       << "Convoys        = " << country.convoys_ << "\n"
       << "Units          = " << country.units_.size() << "\n"
       << fmt::format("Ideas          = {{ {} }}\n", fmt::join(country.ideas_, " "))
       << fmt::format("Puppets        = {{ {} }}\n", fmt::join(country.puppets_, " "))
       << "NB: Techs, equipment variants, color, graphics block, and name list not printed.\n"
       << "If you are testing one of these, add them to the debug printer in hoi4_country.cpp.\n";
}

void PrintTo(const TaskForce& force, std::ostream* os)
{
   *os << "Location = " << force.location << "\n" << fmt::format("{} ships:\n", force.ships.size());
   for (const auto& ship: force.ships)
   {
      *os << fmt::format("{} {} {} {} {} \n",
          ship.GetName(),
          ship.GetDefinition(),
          ship.GetEquipment(),
          ship.GetLegacyEquipment(),
          ship.GetVersion());
   }
}


}  // namespace hoi4
