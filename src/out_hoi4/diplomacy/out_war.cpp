#include "src/out_hoi4/diplomacy/out_war.h"



std::ostream& out::operator<<(std::ostream& out, const hoi4::War& war)
{
   out << "declare_war_on = {\n";
   out << "\ttarget = " << war.original_defender << "\n";
   out << "\ttype = annex_everything\n";
   out << "}\n";

   for (const std::string& extra_attacker: war.extra_attackers)
   {
      out << extra_attacker << " = {\n";
      out << "\tadd_to_war = {\n";
      out << "\t\ttargeted_alliance = " << war.original_attacker << "\n";
      out << "\t\tenemy = " << war.original_defender << "\n";
      out << "\t}\n";
      out << "}\n";
   }

   for (const auto& extra_defender: war.extra_defenders)
   {
      out << extra_defender << " = {\n";
      out << "\tadd_to_war = {\n";
      out << "\t\ttargeted_alliance = " << war.original_defender << "\n";
      out << "\t\tenemy = " << war.original_attacker << "\n";
      out << "\t}\n";
      out << "}\n";
   }

   out << "\n";

   return out;
}