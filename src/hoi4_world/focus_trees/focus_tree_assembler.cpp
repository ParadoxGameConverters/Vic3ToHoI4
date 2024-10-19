#include "src/hoi4_world/focus_trees/focus_tree_assembler.h"

#include <ranges>



hoi4::FocusTree hoi4::AssembleTree(const std::vector<Role>& roles, std::string_view tag, const hoi4::World& world)
{
   FocusTree tree;

   std::map<std::string, std::vector<std::string>> role_lookup;
   for (const Role& role: roles)
   {
      const std::vector<std::string>& shared_focuses = role.GetSharedFocuses();
      tree.shared_focuses.insert(tree.shared_focuses.end(), shared_focuses.begin(), shared_focuses.end());

      const std::vector<Focus>& focuses = role.GetFocuses();
      tree.focuses.insert(tree.focuses.end(), focuses.begin(), focuses.end());

      for (const RepeatFocus& repeat_focus: role.GetRepeatFocuses())
      {
         int num_targets = 0;

         std::map<std::string, std::vector<Focus>> target_focuses;
         for (const auto& [target_tag, country]: world.GetCountries())
         {
            if (!repeat_focus.requirement(country, world))
            {
               continue;
            }

            ++num_targets;

            for (Focus focus_copy: repeat_focus.focuses)
            {
               std::string original_id = focus_copy.id;
               while (focus_copy.id.find("$TARGET_TAG$") != std::string::npos)
               {
                  focus_copy.id.replace(focus_copy.id.find("$TARGET_TAG$"), 12, target_tag);
               }
               for (std::string& prerequisite: focus_copy.prerequisites)
               {
                  while (prerequisite.find("$TARGET_TAG$") != std::string::npos)
                  {
                     prerequisite.replace(prerequisite.find("$TARGET_TAG$"), 12, target_tag);
                  }
               }
               if (focus_copy.relative_position_id.has_value())
               {
                  while (focus_copy.relative_position_id->find("$TARGET_TAG$") != std::string::npos)
                  {
                     focus_copy.relative_position_id->replace(focus_copy.relative_position_id->find("$TARGET_TAG$"),
                         12,
                         target_tag);
                  }
               }
               if (focus_copy.available.has_value())
               {
                  while (focus_copy.available->find("$TARGET_TAG$") != std::string::npos)
                  {
                     focus_copy.available->replace(focus_copy.available->find("$TARGET_TAG$"), 12, target_tag);
                  }
               }
               if (focus_copy.select_effect.has_value())
               {
                  while (focus_copy.select_effect->find("$TARGET_TAG$") != std::string::npos)
                  {
                     focus_copy.select_effect->replace(focus_copy.select_effect->find("$TARGET_TAG$"), 12, target_tag);
                  }
               }
               while (focus_copy.completion_reward.find("$TARGET_TAG$") != std::string::npos)
               {
                  focus_copy.completion_reward.replace(focus_copy.completion_reward.find("$TARGET_TAG$"),
                      12,
                      target_tag);
               }
               if (focus_copy.ai_will_do.has_value())
               {
                  while (focus_copy.ai_will_do->find("$TARGET_TAG$") != std::string::npos)
                  {
                     focus_copy.ai_will_do->replace(focus_copy.ai_will_do->find("$TARGET_TAG$"), 12, target_tag);
                  }
               }

               role_lookup[original_id].push_back(focus_copy.id);
               if (auto [itr, success] = target_focuses.emplace(original_id, std::vector{focus_copy}); !success)
               {
                  itr->second.push_back(focus_copy);
               }
            }
         }


         for (std::vector<Focus>& focuses: target_focuses | std::views::values)
         {
            int x_position = 1 - num_targets;
            int targets_addressed = 0;
            for (Focus& focus: focuses)
            {
               focus.x_position = x_position;
               tree.focuses.push_back(focus);

               x_position += 2;
               ++targets_addressed;
               if (targets_addressed == num_targets)
               {
                  x_position = 1 - num_targets;
                  targets_addressed = 0;
               }
            }
         }
      }
   }

   int position = tree.shared_focuses.size() * 10;
   for (Focus& focus: tree.focuses)
   {
      if (focus.tree_starter)
      {
         focus.x_position = position;
         position += 10;
      }

      while (focus.id.find("$TAG$") != std::string::npos)
      {
         focus.id.replace(focus.id.find("$TAG$"), 5, tag);
      }
      if (focus.relative_position_id.has_value())
      {
         while (focus.relative_position_id->find("$TAG$") != std::string::npos)
         {
            focus.relative_position_id->replace(focus.relative_position_id->find("$TAG$"), 5, tag);
         }
      }
      if (focus.ai_will_do.has_value())
      {
         while (focus.ai_will_do->find("$TAG$") != std::string::npos)
         {
            focus.ai_will_do->replace(focus.ai_will_do->find("$TAG$"), 5, tag);
         }
      }
   }

   return tree;
}