#include "src/out_hoi4/technology/out_technologies.h"

#include <external/fmt/include/fmt/format.h>



std::ostream& out::operator<<(std::ostream& out, const hoi4::Technologies& all_technologies)
{
   const auto& technologies_by_limit = all_technologies.GetTechnologies();

   out << "# Starting tech\n";

   // technologies with no limit have a different output format, so handle them separately
   if (const auto& no_limit_techs = technologies_by_limit.find(std::nullopt);
       no_limit_techs != technologies_by_limit.end())
   {
      out << "set_technology = {\n";
      for (const auto& technology: no_limit_techs->second)
      {
         out << fmt::format("\t{} = 1\n", technology);
      }
      out << "}\n";
      out << "\n";
   }

   for (const auto& [limit, technologies]: technologies_by_limit)
   {
      if (!limit.has_value())  // no limit techs are handled above
      {
         continue;
      }
      if (technologies.empty())
      {
         continue;
      }

      out << "if = {\n";
      out << fmt::format("\tlimit = {{ {} }}\n", *limit);
      out << "\tset_technology = {\n";
      for (const std::string& technology: technologies)
      {
         out << fmt::format("\t\t{} = 1\n", technology);
      }
      out << "\t}\n";
      out << "}\n";
      out << "\n";
   }

   return out;
}