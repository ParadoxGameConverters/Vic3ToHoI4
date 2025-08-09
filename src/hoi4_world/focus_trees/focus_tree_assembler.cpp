#include "src/hoi4_world/focus_trees/focus_tree_assembler.h"

#include <ranges>

#include "src/hoi4_world/roles/triggers/context.h"



namespace
{

// For all repeated focuses defined them into the role, expand them into actual focuses.
// Also add to a lookup map of the original repeat focus id to the resulting focus ids.
std::vector<hoi4::Focus> CreateRepeatedFocuses(const hoi4::Role& role,
    const hoi4::World& world,
    std::map<std::string, std::vector<std::string>>& role_lookup)
{
   std::vector<hoi4::Focus> repeated_focuses;

   for (const hoi4::RepeatFocus& repeat_focus: role.GetRepeatFocuses())
   {
      // Track how many countries are applicable so that the focuses can be balanced in position.
      int num_targets = 0;

      // Generate all the focuses
      std::map<std::string, std::vector<hoi4::Focus>> target_focuses;
      for (const auto& [target_tag, country]: world.GetCountries())
      {
         hoi4::CountryScope country_scope(country);
         hoi4::Context context{
             .root = country_scope,
             .this_scope = country_scope,
             .prev = country_scope,
             .from = country_scope,
         };
         if (!repeat_focus.GetTrigger().IsValid(context, world))
         {
            continue;
         }

         ++num_targets;

         for (hoi4::Focus focus_copy: repeat_focus.GetFocuses())
         {
            std::string original_id = focus_copy.id;
            focus_copy.ApplyReplacement("$TARGET_TAG$", target_tag);

            role_lookup[original_id].push_back(focus_copy.id);
            if (auto [itr, success] = target_focuses.emplace(original_id, std::vector{focus_copy}); !success)
            {
               itr->second.push_back(focus_copy);
            }
         }
      }

      // Properly position all the focuses
      for (std::vector<hoi4::Focus>& focuses: target_focuses | std::views::values)
      {
         int x_position = 1 - num_targets;
         int targets_addressed = 0;
         for (hoi4::Focus& focus: focuses)
         {
            focus.x_position = x_position;
            repeated_focuses.push_back(focus);

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

   return repeated_focuses;
}


// For any prerequisites that are of the form repeat_focus = <a repeat focus>, expand them to be normal prerequisites
// than include all of the focuses that the repeat focus had been expanded into
void UpdatePrerequisites(const std::map<std::string, std::vector<std::string>>& role_lookup, hoi4::Focus& focus)
{
   for (std::string& prerequisite: focus.prerequisites)
   {
      if (!prerequisite.contains("repeat_focus"))
      {
         continue;
      }

      std::string prerequisite_string = prerequisite;

      // determine the text to replace
      std::string to_replace;
      std::regex prerequisite_regex(R"(.*(repeat_focus = [^\s]+).*)");
      if (std::smatch match; std::regex_match(prerequisite_string, match, prerequisite_regex))
      {
         to_replace = match.str(1);
      }

      // determine the replacement text
      std::string replace_with;
      std::regex lookup_regex(R"(.*repeat_focus =\s([^\s]+).*)");
      if (std::smatch match; std::regex_match(prerequisite_string, match, lookup_regex))
      {
         if (auto role = role_lookup.find(match.str(1)); role != role_lookup.end())
         {
            // while we're here, position this focus evenly under the repeat focuses
            if (!role->second.empty())
            {
               if ((role->second.size() % 2) == 0)
               {
                  focus.relative_position_id = role->second.at(role->second.size() / 2 - 1);
                  focus.x_position = 1;
               }
               else
               {
                  focus.relative_position_id = role->second.at(role->second.size() / 2);
                  focus.x_position = 0;
               }
            }

            for (auto& new_id: role->second)
            {
               if (replace_with.empty())
               {
                  replace_with = fmt::format("focus = {}", new_id);
               }
               else
               {
                  replace_with = fmt::format("{} focus = {}", replace_with, new_id);
               }
            }
         }
      }

      // actually do the text replacement
      while (prerequisite.find(to_replace) != std::string::npos)
      {
         prerequisite.replace(prerequisite.find(to_replace), to_replace.size(), replace_with);
      }
   }
}

}  // namespace



hoi4::FocusTree hoi4::AssembleTree(const std::vector<Role>& roles, std::string_view tag, const hoi4::World& world)
{
   FocusTree tree;

   std::map<std::string, std::vector<std::string>> role_lookup;
   for (const Role& role: roles)
   {
      // add all shared focuses to the tree
      const std::vector<std::string>& shared_focuses = role.GetSharedFocuses();
      tree.shared_focuses.insert(tree.shared_focuses.end(), shared_focuses.begin(), shared_focuses.end());

      // add all regular focuses to the tree
      const std::vector<Focus>& focuses = role.GetFocuses();
      tree.focuses.insert(tree.focuses.end(), focuses.begin(), focuses.end());

      // add all repeated focuses to the tree
      const std::vector<Focus> repeated_focuses = CreateRepeatedFocuses(role, world, role_lookup);
      tree.focuses.insert(tree.focuses.end(), repeated_focuses.begin(), repeated_focuses.end());
   }

   // make final updates to the focuses - positions, fixes to prerequisites, and replace $TAG$ with the actual tag
   int position = static_cast<int>(tree.shared_focuses.size()) * 10;
   for (Focus& focus: tree.focuses)
   {
      if (focus.tree_starter)
      {
         focus.x_position = position;
         position += 10;
      }

      UpdatePrerequisites(role_lookup, focus);
      focus.ApplyReplacement("$TAG$", tag);
   }

   return tree;
}