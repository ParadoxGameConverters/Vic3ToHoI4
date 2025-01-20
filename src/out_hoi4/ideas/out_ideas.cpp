#include "src/out_hoi4/ideas/out_ideas.h"

#include <external/fmt/include/fmt/format.h>

#include <fstream>
#include <ranges>

#include "src/hoi4_world/characters/hoi4_characters_converter.h"



using std::filesystem::path;



void out::OutputMonarchIdeas(const path& output_name,
    const std::map<std::string, hoi4::Country>& countries,
    const std::map<int, hoi4::Character>& characters)
{
   const path monarch_ideas_file_name = "output" / output_name / "common/ideas/_monarchs.txt";
   std::ofstream monarch_ideas(monarch_ideas_file_name);
   if (!monarch_ideas.is_open())
   {
      throw std::runtime_error(fmt::format("Could not create {}", monarch_ideas_file_name.string()));
   }

   monarch_ideas << "ideas = {\n";
   monarch_ideas << "\tcountry = {\n";

   for (const auto& [tag, country]: countries)
   {
      const std::optional<int64_t>& monarch_idea_id = country.GetMonarchIdeaIds();
      if (!monarch_idea_id.has_value())
      {
         continue;
      }

      const auto monarch_itr = characters.find(*monarch_idea_id);
      if (monarch_itr == characters.end())
      {
         continue;
      }

      const std::string monarch_idea_name = hoi4::GetMonarchIdeaName(tag, monarch_itr->second);

      monarch_ideas << fmt::format("\t\t{} = {{\n", monarch_idea_name);
      monarch_ideas << "\t\t\tallowed = {\n";
      monarch_ideas << fmt::format("\t\t\t\toriginal_tag = {}\n", tag);
      monarch_ideas << "\t\t\t}\n";
      monarch_ideas << "\n";
      monarch_ideas << "\t\t\tallowed_civil_war = {\n";
      monarch_ideas << fmt::format("\t\t\t\toriginal_tag = {}\n", tag);
      monarch_ideas << "\t\t\t\thas_government = democratic\n";
      monarch_ideas << "\t\t\t}\n";
      monarch_ideas << "\n";
      monarch_ideas << "\t\t\tremoval_cost = -1\n";
      monarch_ideas << "\n";
      monarch_ideas << "\t\t\ttraits = { popular_figurehead }\n";
      monarch_ideas << "\t\t}\n";
   }

   monarch_ideas << "\t}\n";
   monarch_ideas << "}\n";

   monarch_ideas.close();
}