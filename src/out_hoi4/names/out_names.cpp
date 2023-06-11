#include "src/out_hoi4/names/out_names.h"

#include <fstream>

#include "external/fmt/include/fmt/format.h"

void out::OutputNames(std::string_view output_name, const std::vector<hoi4::Country>& countries)
{
   const auto file_path = fmt::format("output/{}/common/names/converter_names.txt", output_name);
   std::ofstream out(file_path);
   if (!out.is_open())
   {
      throw std::runtime_error(fmt::format("Could not open {}", file_path));
   }

   for (const auto& country: countries)
   {
      const auto& name_list = country.GetNameList();

      // out << fmt::format("{} = {\n", country.GetTag());
      out << "\tmale = {\n";
      out << "\t\tnames = {\n";
      // out << name_list.male_names;
      if (!name_list.male_surnames.empty())
      {
         out << "\t\tsurnames = {\n";
         // out << name_list.male_surnames;
         out << "\t\t}\n";
      }
      out << "\t\t}\n";
      out << "\t}\n";
      out << "\tfemale = {\n";
      out << "\t\tnames = {\n";
      // out << name_list.female_names;
      out << "\t\t}\n";
      if (!name_list.female_surnames.empty())
      {
         out << "\t\tsurnames = {\n";
         // out << name_list.female_surnames;
         out << "\t\t}\n";
      }
      out << "\t}\n";
      out << "\tsurnames = {\n";
      out << "\t\tnames = {\n";
      // out << name_list.surnames;
      out << "\t\t}\n";
      out << "\t}\n";
      out << "}";
   }

   out.close();
}
