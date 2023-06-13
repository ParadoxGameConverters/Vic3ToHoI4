#include "src/out_hoi4/names/out_name_list.h"

#include "external/fmt/include/fmt/format.h"

namespace
{
void OutNameSet(std::ostream& out, const std::set<std::string>& name_set)
{
   int names = 0;
   out << "\t\t\t";

   for (const auto& name: name_set)
   {
      ++names;
      if (names % 12 == 0)  // Make it easier for human reading in plaintext
      {
         out << "\n\t\t\t";
      }
      out << fmt::format("\"{}\" ", name);
   }
   out << "\n";
}
}  // namespace

std::ostream& hoi4::operator<<(std::ostream& out, const NameList& name_list)
{
   out << "\tmale = {\n";
   out << "\t\tnames = {\n";
   OutNameSet(out, name_list.male_names);
   out << "\t\t}\n";
   if (!name_list.male_surnames.empty())
   {
      out << "\t\tsurnames = {\n";
      OutNameSet(out, name_list.male_surnames);
      out << "\t\t}\n";
   }
   out << "\t}\n";
   out << "\tfemale = {\n";
   out << "\t\tnames = {\n";
   OutNameSet(out, name_list.female_names);
   out << "\t\t}\n";
   if (!name_list.female_surnames.empty())
   {
      out << "\t\tsurnames = {\n";
      OutNameSet(out, name_list.female_surnames);
      out << "\t\t}\n";
   }
   out << "\t}\n";
   out << "\tsurnames = {\n";
   OutNameSet(out, name_list.surnames);
   out << "\t}\n";
   return out;
}
