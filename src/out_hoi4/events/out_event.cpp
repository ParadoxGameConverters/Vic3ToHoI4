#include "src/out_hoi4/events/out_event.h"

#include "src/out_hoi4/events/out_event_option.h"



namespace out
{

std::ostream& operator<<(std::ostream& out, const hoi4::Event& event)
{
   out << event.type << " = {\n";
   out << "\tid = " << event.id << "\n";
   if (event.title)
   {
      out << "\ttitle = " << *event.title << "\n";
   }
   for (const auto& description: event.descriptions)
   {
      out << "\tdesc " << description << "\n";
   }
   if (event.picture)
   {
      out << "\tpicture = " << *event.picture << "\n";
   }
   if (event.major_event)
   {
      out << "\t\n";
      out << "\tmajor = yes\n";
   }
   out << "\n";
   if (event.triggered_only)
   {
      out << "\tis_triggered_only = yes\n";
   }
   if (event.hidden)
   {
      if (*event.hidden)
      {
         out << "\thidden = yes\n";
      }
      else
      {
         out << "\thidden = no\n";
      }
   }

   if (!event.trigger.empty())
   {
      out << "\n";
      out << "\ttrigger " << event.trigger << "\n";
   }

   if (event.fire_only_once)
   {
      out << "\n";
      if (*event.fire_only_once)
      {
         out << "\tfire_only_once = yes\n";
      }
      else
      {
         out << "\tfire_only_once = no\n";
      }
   }

   if (!event.mean_time_to_happen.empty())
   {
      out << "\n";
      out << "\tmean_time_to_happen " << event.mean_time_to_happen << "\n";
   }

   if (!event.immediate.empty())
   {
      out << "\n";
      out << "\timmediate " << event.immediate << "\n";
   }

   for (const auto& option: event.options)
   {
      out << "\n";
      out << option << "\n";
   }

   out << "}\n";

   return out;
}

}  // namespace out