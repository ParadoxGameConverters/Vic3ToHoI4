#include <OSCompatibilityLayer.h>
#include <Parser.h>
#include <fmt/format.h>
#include <parserHelpers.h>

#include <filesystem>
#include <fstream>
#include <ostream>
#include <print>
#include <ranges>



class State
{
  public:
   explicit State(const std::filesystem::path& path)
   {
      parser_.registerKeyword("resources", [this](std::istream& input) {
         const commonItems::assignments resources(input);
         for (const auto& [key, value]: resources.getAssignments())
         {
            resources_[key] = std::stoi(value);
         }
      });
      parser_.registerKeyword("provinces", [this](std::istream& input) {
         const commonItems::intList provinces(input);
         provinces_ = provinces.getInts();
      });
      parser_.IgnoreUnregisteredItems();


      state_parser_.registerKeyword("state", [this](std::istream& input) {
         parser_.parseStream(input);
      });
      state_parser_.parseFile(path);
   }

   void AssignResources(std::map<int, std::map<std::string, int>>& province_resource_distribution)
   {
      for (const auto& [resource, amount]: resources_)
      {
         int remaining = amount;
         while (remaining != 0)
         {
            for (const auto& province: provinces_)
            {
               province_resource_distribution[province][resource] += 1;
               --remaining;
               if (remaining == 0)
               {
                  break;
               }
            }
         }
      }
   }

  private:
   commonItems::parser state_parser_;
   commonItems::parser parser_;
   std::map<std::string, int> resources_;
   std::vector<int> provinces_;
};



int main(int argc, [[maybe_unused]] char* argv[])
{
   if (argc < 2)
   {
      std::println("Need to specify Hoi4 install folder");
   }
   else
   {
      std::println("Hoi4 install folder: {}", argv[1]);
   }

   std::map<int, std::map<std::string, int>> province_resource_distribution_map;

   const std::filesystem::path states_folder = std::filesystem::path(argv[1]) / "history/states";
   for (const auto& file: commonItems::GetAllFilesInFolder(states_folder))
   {
      const std::filesystem::path full_path = states_folder / file;
      State state(full_path);
      state.AssignResources(province_resource_distribution_map);
   }

   std::vector<std::pair<int, std::map<std::string, int>>> sorted_province_resource_distribution;
   for (const auto& [province, resources]: province_resource_distribution_map)
   {
      sorted_province_resource_distribution.push_back({province, resources});
   }
   std::ranges::sort(sorted_province_resource_distribution,
       [](const std::pair<int, std::map<std::string, int>>& a, const std::pair<int, std::map<std::string, int>>& b) {
          return a.first < b.first;
       });

   std::ofstream output("resources.txt");
   for (const auto& [province, resources]: sorted_province_resource_distribution)
   {
      std::println("outputting province {}", province);
      std::print(output, "link = {{ province = {} resources = {{", province);
      for (const auto& [resource, amount]: resources)
      {
         std::print(output, " {} = {}", resource, amount);
      }
      std::println(output, " }} }}");
   }
   output.close();

   return 0;
}