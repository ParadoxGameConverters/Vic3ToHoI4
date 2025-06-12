#include "src/hoi4_world/decisions/decisions_category.h"

#include <ostream>



namespace hoi4
{

void PrintTo(const DecisionsCategory& decisions_category, std::ostream* os)
{
   *os << "DecisionsCategory {\n";
   *os << "name - " << decisions_category.name;
   *os << "icon - " << decisions_category.icon;
   *os << "picture - " << decisions_category.picture;
   if (decisions_category.priority)
   {
      *os << "priority - " << *decisions_category.priority;
   }
   *os << "allowed - " << decisions_category.allowed;
   *os << "visible - " << decisions_category.visible;
   *os << "visibility_type - " << decisions_category.visibility_type;
   for (const auto& [name, item]: decisions_category.extra_items)
   {
      *os << name << "{\n";
      *os << item;
      *os << "}\n";
   }
   *os << "}\n";
}

}  // namespace hoi4