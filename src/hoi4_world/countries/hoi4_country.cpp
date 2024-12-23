#include "src/hoi4_world/countries/hoi4_country.h"

#include <external/fmt/include/fmt/format.h>
#include <external/fmt/include/fmt/ranges.h>

#include <sstream>



namespace hoi4
{

void Country::RemovePuppets(const std::set<std::string>& puppet_tags)
{
   std::set<std::string> puppets_to_keep;

   for (const std::string& puppet: puppets_)
   {
      if (!puppet_tags.contains(puppet))
      {
         puppets_to_keep.insert(puppet);
      }
   }

   puppets_ = puppets_to_keep;
}


void PrintTo(const Country& country, std::ostream* os)
{
   *os << "\n"
       << "Source Country Number = " << country.source_country_number_ << "\n"
       << "Tag                   = " << country.tag_ << "\n"
       << "Capital               = " << country.capital_state_.value_or(-1) << "\n"
       << fmt::format("Primary Cultures      = {{ {} }}\n", fmt::join(country.primary_cultures_, " "))
       << "Ideology              = " << country.ideology_ << " (" << country.sub_ideology_ << ")\n"
       << "Elections             = " << country.last_election_ << " (" << country.has_elections_ << ")\n"
       << "Economy               = " << country.economy_law_ << "\n"
       << "Trade                 = " << country.trade_law_ << "\n"
       << "Military              = " << country.military_law_ << "\n"
       << "Monarch idea          = " << country.monarch_idea_id_.value_or(-1) << "\n"
       << "Overlord              = " << country.overlord_.value_or("none") << "\n"
       << "Research slots        = " << country.starting_research_slots_ << "\n"
       << "Stability             = " << country.stability_ << "\n"
       << "Convoys               = " << country.convoys_ << "\n"
       << "Units                 = " << country.units_.size() << "\n"
       << "Task forces           = " << country.task_forces_.size() << "\n"
       << fmt::format("Ideas                 = {{ {} }}\n", fmt::join(country.ideas_, " "))
       << fmt::format("Puppets               = {{ {} }}\n", fmt::join(country.puppets_, " "))
       << "NB: Techs, equipment variants, color, graphics block, name list, and wars not printed.\n"
       << "If you are testing one of these, add them to the debug printer in hoi4_country.cpp.\n";
}

}  // namespace hoi4
