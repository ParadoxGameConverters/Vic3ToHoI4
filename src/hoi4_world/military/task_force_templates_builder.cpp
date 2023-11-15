#include "src/hoi4_world/military/task_force_templates_builder.h"

#include "external/commonItems/CommonRegexes.h"
#include "external/commonItems/Log.h"
#include "external/commonItems/Parser.h"
#include "external/commonItems/ParserHelpers.h"



std::vector<hoi4::TaskForceTemplate> hoi4::ImportTaskForceTemplates(std::string_view templates_filename)
{
   std::vector<TaskForceTemplate> taskforce_templates;
   std::map<std::string, float> costs;
   std::vector<hoi4::Ship> ships;

   commonItems::parser cost_parser;
   cost_parser.registerRegex(commonItems::catchallRegex, [&costs](const std::string& key, std::istream& input_stream) {
      auto value = commonItems::getDouble(input_stream);
      // Disallow tiny costs to avoid spamming conversion
      // with ships and infinite ship-creation loops.
      if (value < 0.01)
      {
         return;
      }
      costs[key] = value;
   });
   commonItems::parser template_parser;
   template_parser.registerKeyword("cost", [&costs, &cost_parser](std::istream& input_stream) {
      cost_parser.parseStream(input_stream);
   });
   template_parser.registerKeyword("ship", [&ships](std::istream& input_stream) {
      commonItems::simpleObject shipValues(input_stream);
      hoi4::Ship ship(commonItems::remQuotes(shipValues.getValue("name")),
          shipValues.getValue("definition"),
          shipValues.getValue("equipment"),
          shipValues.getValue("legacy_equipment"),
          commonItems::remQuotes(shipValues.getValue("version")));
      if (ship.GetName().empty())
      {
         Log(LogLevel::Warning) << "Ignore ship without name";
         return;
      }
      if (ship.GetDefinition().empty())
      {
         Log(LogLevel::Warning) << "Ignore ship without definition";
         return;
      }
      if (ship.GetEquipment().empty())
      {
         Log(LogLevel::Warning) << "Ignore ship without equipment";
         return;
      }
      if (ship.GetLegacyEquipment().empty())
      {
         Log(LogLevel::Warning) << "Ignore ship without legacy_equipment";
         return;
      }
      if (ship.GetVersion().empty())
      {
         Log(LogLevel::Warning) << "Ignore ship without version";
         return;
      }
      ships.push_back(ship);
   });

   commonItems::parser file_parser;
   file_parser.registerKeyword("task_force",
       [&costs, &ships, &template_parser, &taskforce_templates](std::istream& input_stream) {
          costs.clear();
          ships.clear();
          template_parser.parseStream(input_stream);
          if (costs.empty())
          {
             Log(LogLevel::Warning) << "Ignoring task force template without costs";
             return;
          }
          if (ships.empty())
          {
             Log(LogLevel::Warning) << "Ignoring task force template without ships";
             return;
          }
          taskforce_templates.emplace_back(costs, ships);
       });
   file_parser.registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
   file_parser.parseFile(templates_filename);
   return taskforce_templates;
}
