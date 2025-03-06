#include "src/hoi4_world/decisions/decision.h"



namespace hoi4
{

void PrintTo(const Decision& decision, std::ostream* os)
{
   *os << decision.name;
}

}  // namespace hoi4