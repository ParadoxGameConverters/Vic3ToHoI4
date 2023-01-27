#include <fstream>

#include "external/commonItems/Log.h"
#include "external/commonItems/OSCompatibilityLayer.h"
#include "external/fmt/include/fmt/format.h"
#include "src/out_hoi4/map/out_rocket_sites.h"



void out::OutputRocketSites(std::string_view output_name, const std::vector<hoi4::State>& states)
{
   Log(LogLevel::Info) << "\t\tWriting map info";

   if (!commonItems::TryCreateFolder(fmt::format("output/{}/map", output_name)))
   {
      throw std::runtime_error(fmt::format("Could not create output/{}/map", output_name));
   }

   std::ofstream rocket_sites_file(fmt::format("output/{}/map/rocketsites.txt", output_name));
   if (!rocket_sites_file.is_open())
   {
      throw std::runtime_error(fmt::format("Could not create output/{}/map/rocketsites.txt", output_name));
   }

   for (const auto& state: states)
   {
      if (const auto& provinces = state.GetProvinces(); !provinces.empty())
      {
         rocket_sites_file << state.GetId() << "={" << *provinces.cbegin() << " }\n";
      }
   }

   rocket_sites_file.close();
}