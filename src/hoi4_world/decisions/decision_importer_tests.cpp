#include <external/CommonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

#include "src/hoi4_world/decisions/decision_importer.h"



namespace hoi4
{

TEST(Hoi4worldDecisionsDecisionimporterTests, EverythingDefaultsToEmptyOrBlank)
{
   std::stringstream input;
   input << "= {\n";
   input << "}";
   const Decision decision = DecisionImporter{}.GetDecision("test_name", input);

   EXPECT_EQ(decision.name, "test_name");
   EXPECT_EQ(decision.icon, "");
   EXPECT_EQ(decision.name_field, "");
   EXPECT_EQ(decision.is_good, "");
   EXPECT_EQ(decision.allowed, "");
   EXPECT_EQ(decision.highlight_state_targets, "");
   EXPECT_EQ(decision.available, "");
   EXPECT_EQ(decision.days_mission_timeout, std::nullopt);
   EXPECT_EQ(decision.activation, "");
   EXPECT_EQ(decision.targets, "");
   EXPECT_EQ(decision.target_array, "");
   EXPECT_EQ(decision.target_root_trigger, "");
   EXPECT_EQ(decision.state_target, "");
   EXPECT_EQ(decision.target_trigger, "");
   EXPECT_EQ(decision.target_non_existing, false);
   EXPECT_EQ(decision.remove_trigger, "");
   EXPECT_EQ(decision.custom_cost_trigger, "");
   EXPECT_EQ(decision.custom_cost_text, "");
   EXPECT_EQ(decision.visible, "");
   EXPECT_EQ(decision.cancel_trigger, "");
   EXPECT_EQ(decision.on_map_mode, "");
   EXPECT_EQ(decision.complete_effect, "");
   EXPECT_EQ(decision.targeted_modifier, "");
   EXPECT_EQ(decision.remove_effect, "");
   EXPECT_EQ(decision.timeout_effect, "");
   EXPECT_EQ(decision.ai_will_do, "");
   EXPECT_EQ(decision.fire_only_once, "");
   EXPECT_EQ(decision.days_remove, std::nullopt);
   EXPECT_EQ(decision.days_reenable, std::nullopt);
   EXPECT_EQ(decision.cost, std::nullopt);
   EXPECT_EQ(decision.modifier, "");
}


TEST(Hoi4worldDecisionsDecisionimporterTests, ItemsCanBeSet)
{
   std::stringstream input;
   input << "= {\n";
   input << "\ticon = generic_civil_support\n";
   input << "\tname = recruit_in_europe_state\n";
   input << "\tis_good = no\n";
   input << "\tallowed = {\n";
   input << "\t\t\talways = yes\n";
   input << "\t}\n";
   input << "\thighlight_state_targets = {\n";
   input << "\t\tstate = 105\n";
   input << "\t}\n";
   input << "\tavailable = {\n";
   input << "\t\thas_unsupported_economic_law = yes\n";
   input << "\t\thas_war = no\n";
   input << "\t}\n";
   input << "\tdays_mission_timeout = 90\n";
   input << "\tactivation = {\n";
   input << "\t\thas_unsupported_economic_law = yes\n";
   input << "\t\thas_war = no\n";
   input << "\t}\n";
   input << "\ttargets = { host }\n";
   input << "\ttarget_array = exiles\n";
   input << "\t\ttarget_root_trigger = {\n";
   input << "\t\t\tis_major = yes\n";
   input << "\t\t\thas_capitulated = no\n";
   input << "\t\t}\n";
   input << "\tstate_target = yes\n";
   input << "\ttarget_trigger = {\n";
   input << "\t\tFROM = { \n";
   input << "\t\t\toriginal_tag = GER\n";
   input << "\t\t}\n";
   input << "\t}\n";
   input << "\ttarget_non_existing = yes\n";
   input << "\tremove_trigger = {\n";
   input << "\t\thas_government = fascism\n";
   input << "\t}\n";
   input << "\tcustom_cost_trigger = {\n";
   input << "\t\tcommand_power > 25\n";
   input << "\t}\n";
   input << "\tcustom_cost_text = custom_cost_joint_training_exercise\n";
   input << "\t\tvisible = {\n";
   input << "\t\t\thas_war = no\n";
   input << "\t\t\thas_unsupported_economic_law = yes\n";
   input << "\t\t}\n";
   input << "\tcancel_trigger = {\n";
   input << "\t\thidden_trigger = { \n";
   input << "\t\t\thas_civil_war = yes\n";
   input << "\t\t}\n";
   input << "\t}\n";
   input << "\ton_map_mode = map_only\n";
   input << "\tcomplete_effect = {\n";
   input << "\t\tcountry_event = stability.21\n";
   input << "\t}\n";
   input << "\ttargeted_modifier = {\n";
   input << "\t\ttag = FROM\n";
   input << "\t\ttargeted_legitimacy_daily = -1\n";
   input << "\t\tdemocratic_drift = 0.05\n";
   input << "\t}\n";
   input << "\tremove_effect = {\n";
   input << "\t\tno_effect\n";
   input << "\t}\n";
   input << "\ttimeout_effect = {\n";
   input << "\t\tadd_stability = -0.1 #naughty\n";
   input << "\t}\n";
   input << "\tai_will_do = {\n";
   input << "\t\tfactor = 1\n";
   input << "\t}\n";
   input << "\tfire_only_once = yes\n";
   input << "\tdays_remove = 365\n";
   input << "\tdays_re_enable = 30\n";
   input << "\tcost = var:war_propaganda_cost?75\n";
   input << "\tmodifier = {\n";
   input << "\t\tfascism_drift = 0.02\n";
   input << "\t\tstability_weekly = -0.03\n";
   input << "\t\tpolitical_power_gain = -0.2\n";
   input << "\t}\n";

   input << "}";
   const Decision decision = DecisionImporter{}.GetDecision("test_name", input);

   EXPECT_EQ(decision.name, "test_name");
   EXPECT_EQ(decision.icon, "generic_civil_support");
   EXPECT_EQ(decision.name_field, "recruit_in_europe_state");
   EXPECT_EQ(decision.is_good, "no");
   EXPECT_EQ(decision.allowed,
       "= {\n"
       "\t\t\talways = yes\n"
       "\t}");
   EXPECT_EQ(decision.highlight_state_targets,
       "= {\n"
       "\t\tstate = 105\n"
       "\t}");
   EXPECT_EQ(decision.available,
       "= {\n"
       "\t\thas_unsupported_economic_law = yes\n"
       "\t\thas_war = no\n"
       "\t}");
   EXPECT_TRUE(decision.days_mission_timeout.has_value());
   EXPECT_EQ(decision.days_mission_timeout.value_or(0), 90);
   EXPECT_EQ(decision.activation,
       "= {\n"
       "\t\thas_unsupported_economic_law = yes\n"
       "\t\thas_war = no\n"
       "\t}");
   EXPECT_EQ(decision.targets, "= { host }");
   EXPECT_EQ(decision.target_array, "= exiles");
   EXPECT_EQ(decision.target_root_trigger,
       "= {\n"
       "\t\t\tis_major = yes\n"
       "\t\t\thas_capitulated = no\n"
       "\t\t}");
   EXPECT_EQ(decision.state_target, "yes");
   EXPECT_EQ(decision.target_trigger,
       "= {\n"
       "\t\tFROM = { \n"
       "\t\t\toriginal_tag = GER\n"
       "\t\t}\n"
       "\t}");
   EXPECT_EQ(decision.target_non_existing, true);
   EXPECT_EQ(decision.remove_trigger,
       "= {\n"
       "\t\thas_government = fascism\n"
       "\t}");
   EXPECT_EQ(decision.custom_cost_trigger,
       "= {\n"
       "\t\tcommand_power > 25\n"
       "\t}");
   EXPECT_EQ(decision.custom_cost_text, "= custom_cost_joint_training_exercise");
   EXPECT_EQ(decision.visible,
       "= {\n"
       "\t\t\thas_war = no\n"
       "\t\t\thas_unsupported_economic_law = yes\n"
       "\t\t}");
   EXPECT_EQ(decision.cancel_trigger,
       "= {\n"
       "\t\thidden_trigger = { \n"
       "\t\t\thas_civil_war = yes\n"
       "\t\t}\n"
       "\t}");
   EXPECT_EQ(decision.on_map_mode, "map_only");
   EXPECT_EQ(decision.complete_effect,
       "= {\n"
       "\t\tcountry_event = stability.21\n"
       "\t}");
   EXPECT_EQ(decision.targeted_modifier,
       "= {\n"
       "\t\ttag = FROM\n"
       "\t\ttargeted_legitimacy_daily = -1\n"
       "\t\tdemocratic_drift = 0.05\n"
       "\t}");
   EXPECT_EQ(decision.remove_effect,
       "= {\n"
       "\t\tno_effect\n"
       "\t}");
   EXPECT_EQ(decision.timeout_effect,
       "= {\n"
       "\t\tadd_stability = -0.1 #naughty\n"
       "\t}");
   EXPECT_EQ(decision.ai_will_do,
       "= {\n"
       "\t\tfactor = 1\n"
       "\t}");
   EXPECT_EQ(decision.fire_only_once, "yes");
   EXPECT_TRUE(decision.days_remove.has_value());
   EXPECT_EQ(decision.days_remove.value_or(""), "365");
   EXPECT_TRUE(decision.days_reenable.has_value());
   EXPECT_EQ(decision.days_reenable.value_or(0), 30);
   EXPECT_TRUE(decision.cost.has_value());
   EXPECT_EQ(decision.cost.value_or(""), "var:war_propaganda_cost?75");
   EXPECT_EQ(decision.modifier,
       "= {\n"
       "\t\tfascism_drift = 0.02\n"
       "\t\tstability_weekly = -0.03\n"
       "\t\tpolitical_power_gain = -0.2\n"
       "\t}");
}

}  // namespace hoi4