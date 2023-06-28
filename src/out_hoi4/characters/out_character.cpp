#include "src/out_hoi4/characters/out_character.h"

#include <fstream>
#include <ranges>

#include "external/fmt/include/fmt/format.h"



void out::OutputCharacter(std::ostream& out, const std::string& tag, const hoi4::Character& character)
{
   out << fmt::format("\t{}_{}={\n", tag, character.GetId());
   out << fmt::format("\t\t{}_{}\n", character.GetFirstName(), character.GetLastName());
   out << "\t}\n";
}