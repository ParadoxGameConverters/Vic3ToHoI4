#include "src/out_hoi4/decisions/out_decisions.h"

#include <external/commonItems/Log.h>
#include <external/fmt/include/fmt/format.h>

#include <fstream>

#include "src/out_hoi4/decisions/out_decision.h"


namespace out
{

void OutputDecisions(const std::filesystem::path& output_name,
    const std::map<std::string, std::vector<hoi4::Decision>>& decisions_in_categories)
{
   const std::filesystem::path output_file = "output" / output_name / "common" / "decisions/stories_decisions.txt";
   std::ofstream out(output_file);
   if (!out.is_open())
   {
      Log(LogLevel::Warning) << fmt::format("Could not open {}", output_file.string());
   }

   for (const auto& [category, decisions]: decisions_in_categories)
   {
      out << category << " = {\n\n";
      for (const hoi4::Decision& decision: decisions)
      {
         out << decision << "\n";
      }
      out << "\n}\n\n";
   }

   out.close();
}

}  // namespace out
