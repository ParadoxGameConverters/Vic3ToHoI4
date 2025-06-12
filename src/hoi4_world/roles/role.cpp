#include "src/hoi4_world/roles/role.h"



namespace hoi4
{

void PrintTo(const Role& role, std::ostream* os)
{
   *os << "\nrole:";
   *os << "name - " << role.name_ << "\n";
   *os << "category - " << role.category_ << "\n";
   *os << "requirement - ";
   if (role.requirement_)
   {
      *os << "{\n";
      PrintTo(*role.requirement_, os);
      *os << "}\n";
   }
   else
   {
      *os << "none\n";
   }
   *os << "name - " << role.score_ << "\n";
   *os << "blockers {\n";
   for (std::string_view blocker: role.blockers_)
   {
      *os << blocker << "\n";
   }
   *os << "}\n";
   *os << "shared_focuses {\n";
   for (std::string_view shared_focus: role.shared_focuses_)
   {
      *os << shared_focus << "\n";
   }
   *os << "}\n";
   *os << "focuses {\n";
   for (const Focus& focus: role.focuses_)
   {
      PrintTo(focus, os);
      *os << "\n";
   }
   *os << "}\n";
   *os << "repeat_focuses {\n";
   for (const RepeatFocus& repeat_focus: role.repeat_focuses_)
   {
      PrintTo(repeat_focus, os);
      *os << "\n";
   }
   *os << "}\n";
   *os << "removed_focuses {\n";
   for (std::string_view removed_focus: role.removed_focuses_)
   {
      *os << removed_focus << "\n";
   }
   *os << "}\n";
   *os << "decisions_categories {\n";
   for (const DecisionsCategory& decisions_category: role.decisions_categories_)
   {
      PrintTo(decisions_category, os);
      *os << "\n";
   }
   *os << "}\n";
   for (const auto& [category, decisions]: role.decisions_in_categories_)
   {
      *os << "decisions_in_category - " << category << "{\n";
      for (const Decision& decision: decisions)
      {
         PrintTo(decision, os);
      }
      *os << "}\n";
   }
   std::map<std::string, std::vector<Decision>> decisions_in_categories_;
   *os << "}\n";
   *os << "events {\n";
   for (std::string_view event: role.events_)
   {
      *os << event << "\n";
   }
   *os << "}\n";
}

}  // namespace hoi4