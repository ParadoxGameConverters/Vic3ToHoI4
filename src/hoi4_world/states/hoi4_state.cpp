#include "src/hoi4_world/states/hoi4_state.h"

#include <ostream>

#include "external/fmt/include/fmt/ranges.h"



namespace hoi4
{

void hoi4::State::SetHighestVictoryPointValue(int value)
{
   if (victory_points_.empty())
   {
      return;
   }

   auto highest_value_vp =
       std::ranges::max_element(victory_points_, [this](std::pair<int, int> a, std::pair<int, int> b) {
          return a.second < b.second;
       });
   highest_value_vp->second = value;
}


void PrintTo(const State& state, std::ostream* os)
{
   *os << "\n";
   if (state.owner_.has_value())
   {
      *os << "Owner                 = " << state.owner_.value() << "\n";
   }
   *os << "Provinces            = { ";
   for (const auto& province: state.provinces_)
   {
      *os << province << " ";
   }
   *os << " }\n";
   *os << "Manpower              = " << state.manpower_ << "\n";
   *os << "Resources             = {\n";
   for (const auto& [resource, amount]: state.resources_)
   {
      *os << " " << resource << ": " << amount << "\n";
   }
   *os << " }\n";
   *os << "Category              = " << state.category_ << "\n";
   *os << "Is Capital            = " << (state.is_capital_ ? "yes" : "no") << "\n";
   if (state.continent_.has_value())
   {
      *os << "Continent                 = " << state.continent_.value() << "\n";
   }
   *os << "Victory points             = {\n";
   for (const auto& [position, value]: state.victory_points_)
   {
      *os << " " << position << ": " << value << "\n";
   }
   *os << " }\n";
   *os << "Civilian Factories   = " << state.civilian_factories_ << "\n";
   *os << "Military Factories   = " << state.military_factories_ << "\n";
   *os << "Dockyards             = " << state.dockyards_ << "\n";
   if (state.naval_base_location_.has_value())
   {
      *os << "Naval Base Location  = " << state.naval_base_location_.value() << "\n";
   }
   if (state.naval_base_level_.has_value())
   {
      *os << "Naval Base Level     = " << state.naval_base_level_.value() << "\n";
   }
   *os << "Air Base Level        = " << state.air_base_level_ << "\n";
   *os << "Cores                = { ";
   for (const auto& core: state.cores_)
   {
      *os << core << " ";
   }
   *os << " }\n";
   *os << "Vic3 Infrastructure  = " << state.vic3_infrastructure_ << "\n";
   *os << "Infrastructure       = " << state.infrastructure_ << "\n";
}

}  // namespace hoi4