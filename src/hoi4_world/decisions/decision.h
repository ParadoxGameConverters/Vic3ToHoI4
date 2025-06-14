#ifndef SRC_HOI4WORLD_DECISION_H
#define SRC_HOI4WORLD_DECISION_H


#include <optional>
#include <ostream>
#include <string>



namespace hoi4
{

struct Decision
{
   std::string name;
   std::string icon;
   std::string name_field;
   std::string is_good;
   std::string allowed;
   std::string highlight_state_targets;
   std::string available;
   std::optional<int> days_mission_timeout;
   std::string activation;
   std::string targets;
   std::string target_array;
   std::string target_root_trigger;
   std::string state_target;
   std::string target_trigger;
   bool target_non_existing = false;
   std::string remove_trigger;
   std::string custom_cost_trigger;
   std::string custom_cost_text;
   std::string visible;
   std::string cancel_trigger;
   std::string on_map_mode;
   std::string complete_effect;
   std::string targeted_modifier;
   std::string remove_effect;
   std::string timeout_effect;
   std::string ai_will_do;
   std::string fire_only_once;
   std::optional<std::string> days_remove;
   std::optional<int> days_reenable;
   std::optional<std::string> cost;
   std::string modifier;

   std::strong_ordering operator<=>(const Decision&) const = default;
   friend void PrintTo(const Decision& decision, std::ostream* os);
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_DECISION_H