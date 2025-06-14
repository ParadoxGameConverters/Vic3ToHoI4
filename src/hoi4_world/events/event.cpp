#include "src/hoi4_world/events/event.h"



namespace hoi4
{

void PrintTo(const Event& event, std::ostream* os)
{
   *os << "Type: " << event.type << "\n";
   *os << "ID: " << event.id << "\n";
   if (event.title)
   {
      *os << "Title: " << *event.title << "\n";
   }
   for (std::string_view description: event.descriptions)
   {
      *os << "Description: " << description << "\n";
   }
   if (event.picture)
   {
      *os << "Picture: " << *event.picture << "\n";
   }
   *os << "Major event: " << event.major_event << "\n";
   *os << "Triggered only: " << event.triggered_only << "\n";
   if (event.hidden)
   {
      *os << "Hidden: " << *event.hidden << "\n";
   }
   *os << "Trigger: " << event.trigger << "\n";
   if (event.fire_only_once)
   {
      *os << "Fire only once: " << *event.fire_only_once << "\n";
   }
   *os << "MTTH: " << event.mean_time_to_happen << "\n";
   *os << "Immediate: " << event.immediate << "\n";
   for (const EventOption& option: event.options)
   {
      *os << "Option: ";
      PrintTo(option, os);
      *os << "\n";
   }
}

}  // namespace hoi4