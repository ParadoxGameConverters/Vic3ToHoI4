#include "src/out_hoi4/portraits/out_portrait_paths.h"

#include "external/fmt/include/fmt/format.h"

namespace
{
void OutPathVector(std::ostream& out, const std::vector<std::string>& path_vector)
{
   for (const auto& path: path_vector)
   {
      out << fmt::format("\t\t\t{}\n", path);
   }
}

void OutGenderPathVector(std::ostream& out, const std::string& gender, const std::vector<std::string>& path_vector)
{
   out << fmt::format("\t\t{} = {{\n", gender);
   OutPathVector(out, path_vector);
   out << "\t\t}\n";
}
}  // namespace

std::ostream& mappers::operator<<(std::ostream& out, const PortraitPaths& portrait_paths)
{
   out << "\tarmy = {\n";
   OutGenderPathVector(out, "male", portrait_paths.army);
   out << "\t}\n";
   out << "\tnavy = {\n";
   OutGenderPathVector(out, "male", portrait_paths.navy);
   out << "\t}\n";
   out << "\tpolitical = {\n";
   out << portrait_paths.leader;
   out << "\t}\n";
   out << "\toperative = {\n";
   OutGenderPathVector(out, "male", portrait_paths.male_operative);
   OutGenderPathVector(out, "female", portrait_paths.female_operative);
   out << "\t}\n";
   OutGenderPathVector(out, "female", portrait_paths.female_leader);
   return out;
}

std::ostream& mappers::operator<<(std::ostream& out, const IdeologyPortraitPaths& ideology_portrait_paths)
{
   out << "\t\tcommunism = {\n";
   OutGenderPathVector(out, "male", ideology_portrait_paths.communism);
   out << "\t\t}\n";
   out << "\t\tdemocratic = {\n";
   OutGenderPathVector(out, "male", ideology_portrait_paths.democratic);
   out << "\t\t}\n";
   out << "\t\tfascism = {\n";
   OutGenderPathVector(out, "male", ideology_portrait_paths.fascism);
   out << "\t\t}\n";
   out << "\t\tneutrality = {\n";
   OutGenderPathVector(out, "male", ideology_portrait_paths.neutrality);
   out << "\t\t}\n";
   return out;
}
