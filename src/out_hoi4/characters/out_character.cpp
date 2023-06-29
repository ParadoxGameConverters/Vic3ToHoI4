#include "src/out_hoi4/characters/out_character.h"

#include <fstream>
#include <ranges>

#include "external/fmt/include/fmt/format.h"

namespace
{
std::string OutputTraits(const std::set<std::string>& traits)
{
   std::string traits_string;
   for (const auto& trait: traits)
      traits_string += trait + " ";

   return fmt::format("{{ {}}}", traits_string);
}

void OutputPortraits(std::ostream& out,
    const std::string& portrait_alias,
    const bool is_general,
    const bool is_admiral,
    const bool is_leader,
    const bool is_advisor)
{
   out << "\t\tportraits = {\n";
   if (is_general)
   {
      out << "\t\t\tarmy = {\n";
      out << fmt::format("\t\t\t\tlarge = {}\n", portrait_alias);
      out << fmt::format("\t\t\t\tsmall = {}_small\n", portrait_alias);
      out << "\t\t\t}\n";
   }
   if (is_admiral)
   {
      out << "\t\t\tnavy = {\n";
      out << fmt::format("\t\t\t\tlarge = {}\n", portrait_alias);
      out << fmt::format("\t\t\t\tsmall = {}_small\n", portrait_alias);
      out << "\t\t\t}\n";
   }
   if (is_leader || is_advisor)
   {
      out << "\t\t\tcivilian = {\n";
      if (is_leader)
         out << fmt::format("\t\t\t\tlarge = {}\n", portrait_alias);
      if (!is_general && !is_admiral && is_advisor)
         out << fmt::format("\t\t\t\tsmall = {}\n", portrait_alias);
      out << "\t\t\t}\n";
   }
   out << "\t\t}\n";
}

void OutputLeader(std::ostream& out, const hoi4::Leader& leader_data)
{
   out << "\t\tcountry_leader = {\n";
   out << fmt::format("\t\t\tideology = {}\n", leader_data.sub_ideology);
   out << fmt::format("\t\t\ttraits = {}\n", OutputTraits(leader_data.traits));
   out << "\t\t}\n";
}
void OutputGeneral(std::ostream& out, const hoi4::General& general_data)
{
   if (general_data.is_field_marshal)
   {
      out << "\t\tfield_marshal = {\n";
   }
   else
   {
      out << "\t\tcorps_commander = {\n";
   }
   out << fmt::format("\t\t\ttraits = {}\n", OutputTraits(general_data.traits));
   out << fmt::format("\t\t\tskill = {}\n", general_data.skill);
   out << fmt::format("\t\t\tattack_skill = {}\n", general_data.attack);
   out << fmt::format("\t\t\tdefense_skill = {}\n", general_data.defense);
   out << fmt::format("\t\t\tplanning_skill = {}\n", general_data.planning);
   out << fmt::format("\t\t\tlogistics_skill = {}\n", general_data.logistics);
   out << "\t\t}\n";
}
void OutputAdmiral(std::ostream& out, const hoi4::Admiral& admiral_data)
{
   out << "\t\tnavy_leader = {\n";
   out << fmt::format("\t\t\ttraits = {}\n", OutputTraits(admiral_data.traits));
   out << fmt::format("\t\t\tskill = {}\n", admiral_data.skill);
   out << fmt::format("\t\t\tattack_skill = {}\n", admiral_data.attack);
   out << fmt::format("\t\t\tdefense_skill = {}\n", admiral_data.defense);
   out << fmt::format("\t\t\tmaneuvering_skill = {}\n", admiral_data.maneuvering);
   out << fmt::format("\t\t\tcoordination_skill = {}\n", admiral_data.coordination);
   out << "\t\t}\n";
}
void OutputAdvisor(std::ostream& out, const hoi4::Advisor& advisor_data, const int id)
{
   out << "\t\tadvisor = {\n";
   out << fmt::format("\t\t\ttraits = {}\n", OutputTraits(advisor_data.traits));
   out << fmt::format("\t\t\tslot = {}\n", advisor_data.slot);
   out << fmt::format("\t\t\tledger = {}\n", advisor_data.ledger);
   out << fmt::format("\t\t\tidea_token = i_{}\n", id);
   out << fmt::format("\t\t\tcost = {}\n", advisor_data.cost);
   out << "\t\t}\n";
}
}  // namespace

void out::OutputCharacter(std::ostream& out, const std::string& tag, const hoi4::Character& character)
{
   const auto& general_data = character.GetGeneralData();
   const auto& admiral_data = character.GetAdmiralData();
   const auto& leader_data = character.GetLeaderData();
   const auto& advisor_data = character.GetAdvisorData();

   out << fmt::format("\t{}_{} = {{\n", tag, std::to_string(character.GetId()));
   out << fmt::format("\t\tname = {}_{}\n", character.GetFirstName(), character.GetLastName());
   if (character.IsFemale())
   {
      out << "\t\tgender = female\n";
   }
   OutputPortraits(out,
       character.GetPortraitAlias(),
       general_data.has_value(),
       admiral_data.has_value(),
       leader_data.has_value(),
       advisor_data.has_value());

   if (leader_data)
   {
      OutputLeader(out, leader_data.value());
   }
   if (general_data)
   {
      OutputGeneral(out, general_data.value());
   }
   if (admiral_data)
   {
      OutputAdmiral(out, admiral_data.value());
   }
   if (advisor_data)
   {
      OutputAdvisor(out, advisor_data.value(), character.GetId());
   }
   out << "\t}\n";
}

void out::OutputSpy(std::ostream& out, const hoi4::Character& character)
{
   const auto& spy_data = character.GetSpyData().value();

   out << "\tcreate_operative_leader = {\n";
   out << fmt::format("\t\tname = {}_{}\n", character.GetFirstName(), character.GetLastName());
   out << fmt::format("\t\tGFX = {}\n", character.GetPortraitAlias());
   out << fmt::format("\t\ttraits = {}\n", OutputTraits(spy_data.traits));
   out << "\t\tbypass_recruitment = no\n";
   out << "\t\tavailable_to_spy_master = yes\n";
   if (character.IsFemale())
   {
      out << "\t\tfemale = yes\n";
   }
   out << fmt::format("\t\tnationalities = {}\n", OutputTraits(spy_data.nationalities));
   out << "\t}\n";
}
