#include "src/out_hoi4/focus_trees/out_focus_tree.h"

#include <external/fmt/include/fmt/format.h>

#include <filesystem>
#include <fstream>

#include "src/out_hoi4/focus_trees/out_focus.h"



void out::OutputFocusTree(std::string_view output_name,
    std::string_view tag,
    const hoi4::FocusTree& focus_tree,
    configuration::UseStories use_stories)
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
   for (const std::string& shared_focus: focus_tree.shared_focuses)
   {
      tree_file << fmt::format("\tshared_focus = {}\n", shared_focus);
   }
   if (use_stories == configuration::UseStories::kYes)
   {
      for (const hoi4::Focus& focus: focus_tree.focuses)
      {
         tree_file << focus;
      }
   }
   tree_file << "\n";
   tree_file << "}\n";

   tree_file.close();
}