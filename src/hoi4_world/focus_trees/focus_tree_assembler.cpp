#include "src/hoi4_world/focus_trees/focus_tree_assembler.h"



hoi4::FocusTree hoi4::AssembleTree(const std::vector<Role>& roles, std::string_view tag)
{
   FocusTree tree;

   for (const Role& role: roles)
   {
      const std::vector<std::string>& shared_focuses = role.GetSharedFocuses();
      tree.shared_focuses.insert(tree.shared_focuses.end(), shared_focuses.begin(), shared_focuses.end());

      const std::vector<Focus>& focuses = role.GetFocuses();
      tree.focuses.insert(tree.focuses.end(), focuses.begin(), focuses.end());
   }

   int position = tree.shared_focuses.size() * 10;
   for (Focus& focus: tree.focuses)
   {
      if (!focus.relative_position_id.has_value())
      {
         focus.x_position = position;
         position += 10;
      }

      while (focus.id.find("$TAG$") != std::string::npos)
      {
         focus.id.replace(focus.id.find("$TAG$"), 5, tag);
      }
   }

   return tree;
}