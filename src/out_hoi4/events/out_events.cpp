#include "src/out_hoi4/events/out_events.h"

#include <fstream>

#include "external/commonItems/CommonFunctions.h"
#include "external/commonItems/OSCompatibilityLayer.h"
#include "src/out_hoi4/events/out_event.h"



namespace out
{

void OutputEvents(const std::filesystem::path& output_name,
    const std::map<std::string, std::vector<hoi4::Event>>& country_events)
{
   const auto event_path = "output" / output_name / "events";
   if (!commonItems::DoesFolderExist(event_path) && !std::filesystem::create_directories(event_path))
   {
      throw std::runtime_error("Could not create " + event_path.string());
   }

   for (const auto& [tag, events]: country_events)
   {
      const std::string events_filename = tag + ".txt";
      const std::filesystem::path filename = "output" / output_name / "events" / events_filename;

      std::ofstream out_events(filename);
      if (!out_events.is_open())
      {
         throw std::runtime_error("Could not create " + filename.string());
      }

      out_events << commonItems::utf8BOM;
      out_events << "add_namespace = " << tag << "\n";
      for (const auto& event: events)
      {
         out_events << "\n";
         out_events << event;
      }

      out_events.close();
   }
}

}  // namespace out