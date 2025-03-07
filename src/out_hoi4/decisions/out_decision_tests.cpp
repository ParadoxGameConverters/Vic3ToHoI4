#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

#include "src/hoi4_world/decisions/decision.h"
#include "src/out_hoi4/decisions/out_decision.h"



namespace out
{

TEST(Outhoi4DecisionsOutdecisionTests, EmptyOrBlankItemsAreNotOutput)
{
   const hoi4::Decision decision{.name = "test_name"};

   std::stringstream output;
   output << decision;

   std::stringstream expected_output;
   expected_output << "\n";
   expected_output << "\ttest_name = {\n";
   expected_output << "\t}";
   EXPECT_EQ(expected_output.str(), output.str());
}


TEST(Outhoi4DecisionsOutdecisionTests, ItemsAreOutput)
{
   const hoi4::Decision decision{
       .name = "test_name",
       .icon = "generic_civil_support",
       .is_good = "no",
       .allowed =
           "= {\n"
           "\t\t\talways = yes\n"
           "\t\t}",
       .highlight_state_targets =
           "= {\n"
           "\t\t\tstate = 105\n"
           "\t\t}",
       .available =
           "= {\n"
           "\t\t\thas_unsupported_economic_law = yes\n"
           "\t\t\thas_war = no\n"
           "\t\t}\n"
           "\t}",
       .days_mission_timeout = 90,
       .activation =
           "= {\n"
           "\t\t\thas_unsupported_economic_law = yes\n"
           "\t\t\thas_war = no\n"
           "\t\t}\n",
       .targets = "= { host }",
       .target_array = "= exiles",
       .target_root_trigger =
           "= {\n"
           "\t\t\tis_major = yes\n"
           "\t\t\thas_capitulated = no\n"
           "\t\t}",
       .state_target = "yes",
       .target_trigger =
           "= {\n"
           "\t\t\tFROM = { \n"
           "\t\t\t\toriginal_tag = GER\n"
           "\t\t\t}\n"
           "\t}",
       .target_non_existing = true,
       .remove_trigger =
           "= {\n"
           "\t\thas_government = fascism\n"
           "\t}",
       .custom_cost_trigger =
           "= {\n"
           "\t\t\tcommand_power > 25\n"
           "\t\t}",
       .custom_cost_text = "= custom_cost_joint_training_exercise",
       .visible =
           "= {\n"
           "\t\t\thas_war = no\n"
           "\t\t\thas_unsupported_economic_law = yes\n"
           "\t\t}",
       .cancel_trigger =
           "= {\n"
           "\t\t\thidden_trigger = { \n"
           "\t\t\t\thas_civil_war = yes\n"
           "\t\t\t}\n"
           "\t\t}",
       .on_map_mode = "map_only",
       .complete_effect =
           "= {\n"
           "\t\t\tcountry_event = stability.21\n"
           "\t\t}",
       .targeted_modifier =
           "= {\n"
           "\t\t\ttag = FROM\n"
           "\t\t\ttargeted_legitimacy_daily = -1\n"
           "\t\t\tdemocratic_drift = 0.05\n"
           "\t\t}",
       .remove_effect =
           "= {\n"
           "\t\t\tno_effect\n"
           "\t\t}",
       .timeout_effect =
           "= {\n"
           "\t\t\tadd_stability = -0.1 #naughty\n"
           "\t\t}",
       .ai_will_do =
           "= {\n"
           "\t\t\tfactor = 1\n"
           "\t\t}",
       .fire_only_once = "yes",
       .days_remove = "365",
       .days_reenable = 30,
       .cost = "var:war_propaganda_cost?75",
       .modifier =
           "= {\n"
           "\t\t\tfascism_drift = 0.02\n"
           "\t\t\tstability_weekly = -0.03\n"
           "\t\t\tpolitical_power_gain = -0.2\n"
           "\t\t}",
   };

   std::stringstream output;
   output << decision;

   std::stringstream expected_output;
   expected_output << "\n";
   expected_output << "\ttest_name = {\n";
   expected_output << "\n";
   expected_output << "\t\ticon = generic_civil_support\n";
   expected_output << "\n";
   expected_output << "\t\tis_good = no\n";
   expected_output << "\n";
   expected_output << "\t\tallowed = {\n";
   expected_output << "\t\t\talways = yes\n";
   expected_output << "\t\t}\n";
   expected_output << "\n";
   expected_output << "\t\thighlight_states = {\n";
   expected_output << "\t\t\tstate = 105\n";
   expected_output << "\t\t}\n";
   expected_output << "\n";
   expected_output << "\t\tavailable = {\n";
   expected_output << "\t\t\thas_unsupported_economic_law = yes\n";
   expected_output << "\t\t\thas_war = no\n";
   expected_output << "\t\t}\n";
   expected_output << "\t}\n";
   expected_output << "\n";
   expected_output << "\t\tdays_mission_timeout = 90\n";
   expected_output << "\n";
   expected_output << "\t\tactivation = {\n";
   expected_output << "\t\t\thas_unsupported_economic_law = yes\n";
   expected_output << "\t\t\thas_war = no\n";
   expected_output << "\t\t}\n";
   expected_output << "\n";
   expected_output << "\n";
   expected_output << "\t\ttargets = { host }\n";
   expected_output << "\n";
   expected_output << "\t\ttarget_non_existing = yes\n";
   expected_output << "\n";
   expected_output << "\t\ttarget_array = exiles\n";
   expected_output << "\n";
   expected_output << "\t\ttarget_root_trigger = {\n";
   expected_output << "\t\t\tis_major = yes\n";
   expected_output << "\t\t\thas_capitulated = no\n";
   expected_output << "\t\t}\n";
   expected_output << "\n";
   expected_output << "\t\tstate_target = yes\n";
   expected_output << "\n";
   expected_output << "\t\ttarget_trigger = {\n";
   expected_output << "\t\t\tFROM = { \n";
   expected_output << "\t\t\t\toriginal_tag = GER\n";
   expected_output << "\t\t\t}\n";
   expected_output << "\t}\n";
   expected_output << "\n";
   expected_output << "\t\tvisible = {\n";
   expected_output << "\t\t\thas_war = no\n";
   expected_output << "\t\t\thas_unsupported_economic_law = yes\n";
   expected_output << "\t\t}\n";
   expected_output << "\n";
   expected_output << "\t\tcancel_trigger = {\n";
   expected_output << "\t\t\thidden_trigger = { \n";
   expected_output << "\t\t\t\thas_civil_war = yes\n";
   expected_output << "\t\t\t}\n";
   expected_output << "\t\t}\n";
   expected_output << "\n";
   expected_output << "\t\tremove_trigger = {\n";
   expected_output << "\t\thas_government = fascism\n";
   expected_output << "\t}\n";
   expected_output << "\n";
   expected_output << "\t\tcustom_cost_trigger = {\n";
   expected_output << "\t\t\tcommand_power > 25\n";
   expected_output << "\t\t}\n";
   expected_output << "\n";
   expected_output << "\t\tcustom_cost_text = custom_cost_joint_training_exercise\n";
   expected_output << "\n";
   expected_output << "\t\tfire_only_once = yes\n";
   expected_output << "\n";
   expected_output << "\t\tmodifier = {\n";
   expected_output << "\t\t\tfascism_drift = 0.02\n";
   expected_output << "\t\t\tstability_weekly = -0.03\n";
   expected_output << "\t\t\tpolitical_power_gain = -0.2\n";
   expected_output << "\t\t}\n";
   expected_output << "\n";
   expected_output << "\t\tdays_remove = 365\n";
   expected_output << "\t\tdays_re_enable = 30\n";
   expected_output << "\n";
   expected_output << "\t\tcost = var:war_propaganda_cost?75\n";
   expected_output << "\n";
   expected_output << "\t\ton_map_mode = map_only\n";
   expected_output << "\n";
   expected_output << "\t\tcomplete_effect = {\n";
   expected_output << "\t\t\tcountry_event = stability.21\n";
   expected_output << "\t\t}\n";
   expected_output << "\n";
   expected_output << "\t\ttargeted_modifier = {\n";
   expected_output << "\t\t\ttag = FROM\n";
   expected_output << "\t\t\ttargeted_legitimacy_daily = -1\n";
   expected_output << "\t\t\tdemocratic_drift = 0.05\n";
   expected_output << "\t\t}\n";
   expected_output << "\n";
   expected_output << "\t\tremove_effect = {\n";
   expected_output << "\t\t\tno_effect\n";
   expected_output << "\t\t}\n";
   expected_output << "\n";
   expected_output << "\t\ttimeout_effect = {\n";
   expected_output << "\t\t\tadd_stability = -0.1 #naughty\n";
   expected_output << "\t\t}\n";
   expected_output << "\n";
   expected_output << "\t\tai_will_do = {\n";
   expected_output << "\t\t\tfactor = 1\n";
   expected_output << "\t\t}\n";
   expected_output << "\t}";
   EXPECT_EQ(expected_output.str(), output.str());
}

}  // namespace out