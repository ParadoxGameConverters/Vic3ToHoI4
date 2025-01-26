#include "src/out_hoi4/decisions/out_decisions.h"

#include <external/commonItems/Log.h>


namespace out
{

void OutputDecisions([[maybe_unused]] const std::filesystem::path& output_name,
    [[maybe_unused]] const std::map<std::string, hoi4::Country>& countries)
{
   Log(LogLevel::Debug) << "Decisions not implemented yet, no decisions output.";
}

}  // namespace out
