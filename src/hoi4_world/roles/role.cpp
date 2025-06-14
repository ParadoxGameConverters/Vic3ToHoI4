#include "src/hoi4_world/roles/role.h"



namespace hoi4
{

Role::Role(const Role& rhs)
{
   name_ = rhs.name_;
   category_ = rhs.category_;
   requirement_ = rhs.requirement_->Copy();
   score_ = rhs.score_;
   blockers_ = rhs.blockers_;
   shared_focuses_ = rhs.shared_focuses_;
   focuses_ = rhs.focuses_;
   repeat_focuses_ = rhs.repeat_focuses_;
   removed_focuses_ = rhs.removed_focuses_;
   decisions_categories_ = rhs.decisions_categories_;
   decisions_in_categories_ = rhs.decisions_in_categories_;
   events_ = rhs.events_;
}


Role& Role::operator=(const Role& other)
{
   name_ = other.name_;
   category_ = other.category_;
   requirement_ = other.requirement_->Copy();
   score_ = other.score_;
   blockers_ = other.blockers_;
   shared_focuses_ = other.shared_focuses_;
   focuses_ = other.focuses_;
   repeat_focuses_ = other.repeat_focuses_;
   removed_focuses_ = other.removed_focuses_;
   decisions_categories_ = other.decisions_categories_;
   decisions_in_categories_ = other.decisions_in_categories_;
   events_ = other.events_;

   return *this;
}


std::partial_ordering Role::operator<=>(const Role& other) const
{
   if (name_ != other.name_)
   {
      return name_ <=> other.name_;
   }
   if (category_ != other.category_)
   {
      return category_ <=> other.category_;
   }
   if (!other.requirement_ && requirement_)
   {
      return std::strong_ordering::less;
   }
   if (!requirement_ && other.requirement_)
   {
      return std::strong_ordering::greater;
   }
   if (*requirement_ != *other.requirement_)
   {
      if (*requirement_ < *other.requirement_)
      {
         return std::partial_ordering::less;
      }
      return std::partial_ordering::greater;
   }
   if (score_ != other.score_)
   {
      return score_ <=> other.score_;
   }
   if (blockers_ != other.blockers_)
   {
      return blockers_ <=> other.blockers_;
   }
   if (shared_focuses_ != other.shared_focuses_)
   {
      return shared_focuses_ <=> other.shared_focuses_;
   }
   if (focuses_ != other.focuses_)
   {
      return focuses_ <=> other.focuses_;
   }
   if (repeat_focuses_ != other.repeat_focuses_)
   {
      return repeat_focuses_ <=> other.repeat_focuses_;
   }
   if (removed_focuses_ != other.removed_focuses_)
   {
      return removed_focuses_ <=> other.removed_focuses_;
   }
   if (decisions_categories_ != other.decisions_categories_)
   {
      return decisions_categories_ <=> other.decisions_categories_;
   }
   if (decisions_in_categories_ != other.decisions_in_categories_)
   {
      return decisions_in_categories_ <=> other.decisions_in_categories_;
   }
   if (events_ != other.events_)
   {
      return events_ <=> other.events_;
   }

   return std::strong_ordering::equal;
}


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
   for (const std::string& event: role.events_)
   {
      *os << event << "\n";
   }
   *os << "}\n";
}

}  // namespace hoi4