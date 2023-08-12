#include "src/out_hoi4/national_focus/out_focus_tree.h"

#include <filesystem>
#include <fstream>

#include "external/fmt/include/fmt/format.h"



void out::OutputFocusTree(std::string_view output_name, std::string_view tag)
{
   std::ofstream tree_file(fmt::format("output/{}/common/national_focus/{}_NF.txt", output_name, tag));
   if (!tree_file.is_open())
   {
      throw std::runtime_error(fmt::format("output/{}/common/national_focus/{}.txt", output_name, tag));
   }

   tree_file << "focus_tree = {\n";
   tree_file << fmt::format("\tid = {}_focus\n", tag);
   tree_file << "\t\n";
   tree_file << "\tcountry = {\n";
   tree_file << "\t\tfactor = 0\n";
   tree_file << "\t\t\n";
   tree_file << "\t\tmodifier = {\n";
   tree_file << "\t\t\tadd = 10\n";
   tree_file << fmt::format("\t\t\ttag = {}\n", tag);
   tree_file << "\t\t}\n";
   tree_file << "\t}\n";
   tree_file << "\t\n";
   tree_file << "\tdefault = no\n";
   tree_file << "\treset_on_civilwar = no\n";
   tree_file << "\n";
   tree_file << "\tshared_focus = army_effort\n";
   tree_file << "\tshared_focus = aviation_effort\n";
   tree_file << "\tshared_focus = naval_effort\n";
   tree_file << "\tshared_focus = industrial_effort\n";
   tree_file << "\tshared_focus = political_effort\n";
   tree_file << "\n";
   tree_file << "}\n";

   tree_file.close();
}