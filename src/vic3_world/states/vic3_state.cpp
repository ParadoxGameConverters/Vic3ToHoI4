#include "vic3_state.h"

#include "src/vic3_world/world/vic3_world.h"
namespace vic3
{

[[nodiscard]] std::vector<Building> State::GetBuildings(const World& source_world)
{
   return source_world.GetBuildings().GetBuildingsInState(id_);
}
}  // namespace vic3