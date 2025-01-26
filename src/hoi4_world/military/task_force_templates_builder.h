#ifndef SRC_HOI4WORLD_MILITARY_TASKFORCETEMPLATESBUILDER_H
#define SRC_HOI4WORLD_MILITARY_TASKFORCETEMPLATESBUILDER_H



#include <filesystem>
#include <vector>

#include "src/hoi4_world/military/task_force_template.h"



namespace hoi4
{

std::vector<TaskForceTemplate> ImportTaskForceTemplates(const std::filesystem::path& templates_filename);

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_MILITARY_TASKFORCETEMPLATESBUILDER_H
