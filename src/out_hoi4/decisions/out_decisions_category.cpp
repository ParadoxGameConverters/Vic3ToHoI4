#include "src/out_hoi4/decisions/out_decisions_category.h"



namespace out
{

std::ostream& operator<<(std::ostream& out, const hoi4::DecisionsCategory& decisions_category)
{
   out << decisions_category.name << " = {\n";
   if (!decisions_category.icon.empty())
   {
      out << "\ticon = " << decisions_category.icon << "\n";
   }
   if (!decisions_category.picture.empty())
   {
      out << "\tpicture = " << decisions_category.picture << "\n";
   }
   if (decisions_category.priority)
   {
      out << "\tpriority = " << *decisions_category.priority << "\n";
   }
   if (!decisions_category.allowed.empty())
   {
      out << "\tallowed " << decisions_category.allowed << "\n";
   }
   if (!decisions_category.visible.empty())
   {
      out << "\tvisible " << decisions_category.visible << "\n";
   }
   if (!decisions_category.visibility_type.empty())
   {
      out << "\tvisibility_type " << decisions_category.visibility_type << "\n";
   }
   for (const auto& extraItem: decisions_category.extra_items)
   {
      out << "\t" << extraItem.first << " " << extraItem.second << "\n";
   }
   out << "}\n";
   out << "\n";

   return out;
}

}  // namespace out