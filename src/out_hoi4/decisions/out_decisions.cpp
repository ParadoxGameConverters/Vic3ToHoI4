#include "src/out_hoi4/decisions/out_decisions.h"

#include <external/commonItems/Log.h>


namespace out
{

void OutputDecisions(std::string_view output_name, const std::map<std::string, hoi4::Country>& countries)
{
   Log(LogLevel::Debug) << "Decisions not implemented yet, no decisions output.";
}

}  // namespace out
