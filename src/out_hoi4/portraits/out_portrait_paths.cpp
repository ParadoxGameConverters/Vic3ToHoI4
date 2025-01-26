#include "src/out_hoi4/portraits/out_portrait_paths.h"

#include <external/fmt/include/fmt/format.h>

namespace
{
std::string GetIndentation(const int indentation)
{
   return fmt::format("{:\t>{}}", "", indentation);
}
void CloseBrackets(std::ostream& out, const int indentation)
{
   out << fmt::format("{}}}\n", GetIndentation(indentation));
}

void OutPathVector(std::ostream& out, const std::vector<std::string>& path_vector, const int indentation)
{
   for (const auto& path: path_vector)
   {
      out << fmt::format("{}{}\n", GetIndentation(indentation), path);
   }
}

void OutGenderPathVector(std::ostream& out,
    const std::string& gender,
    const std::vector<std::string>& path_vector,
    const int indentation)
{
   out << fmt::format("{}{} = {{\n", GetIndentation(indentation), gender);
   OutPathVector(out, path_vector, indentation + 1);
   CloseBrackets(out, indentation);
}

void OutIdeologyPathVector(std::ostream& out,
    const std::string& ideology,
    const std::vector<std::string>& path_vector,
    const int indentation)
{
   out << fmt::format("{}{} = {{\n", GetIndentation(indentation), ideology);
   OutGenderPathVector(out, "male", path_vector, indentation + 1);
   CloseBrackets(out, indentation);
}

void OutPortraitCategory(std::ostream& out,
    const mappers::PortraitPaths& portrait_paths,
    const std::string& out_name,
    const int indentation)
{
   out << fmt::format("{}{} = {{\n", GetIndentation(indentation), out_name);
   if (const auto& itr = portrait_paths.find(out_name); itr != portrait_paths.end())
   {
      OutGenderPathVector(out, "male", itr->second, indentation + 1);
   }
   CloseBrackets(out, indentation);
}

void OutPortraitCategory(std::ostream& out,
    const mappers::PortraitPaths& portrait_paths,
    const std::string& out_name,
    const int indentation,
    const std::vector<std::string>& genders)
{
   out << fmt::format("{}{} = {{\n", GetIndentation(indentation), out_name);
   for (const auto& gender: genders)
   {
      const std::string key = fmt::format("{}_{}", out_name, gender);
      if (const auto& itr = portrait_paths.find(key); itr != portrait_paths.end())
      {
         OutGenderPathVector(out, gender, itr->second, indentation + 1);
      }
   }
   CloseBrackets(out, indentation);
}

void OutPoliticalCategories(std::ostream& out, const mappers::PortraitPaths& portrait_paths)
{
   out << "\tpolitical = {\n";
   for (const auto& ideology: {"communism", "democratic", "fascism", "neutrality"})
   {
      const std::string key = fmt::format("leader_{}", ideology);
      if (const auto& itr = portrait_paths.find(key); itr != portrait_paths.end())
      {
         OutIdeologyPathVector(out, ideology, itr->second, 2);
      }
   }
   CloseBrackets(out, 1);
}
}  // namespace

void mappers::OutPortraitPaths(std::ostream& out, const mappers::PortraitPaths& portrait_paths)
{
   OutPortraitCategory(out, portrait_paths, "army", 1);
   OutPortraitCategory(out, portrait_paths, "navy", 1);
   OutPoliticalCategories(out, portrait_paths);
   OutPortraitCategory(out, portrait_paths, "operative", 1, {"male", "female"});
   OutPortraitCategory(out, portrait_paths, "scientist", 1, {"male", "female"});
   if (const auto& itr = portrait_paths.find("female_leader"); itr != portrait_paths.end())
   {
      OutGenderPathVector(out, "female", itr->second, 1);
   }
}