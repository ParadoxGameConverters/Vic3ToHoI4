#ifndef SRC_HOI4WORLD_MILITARY_DIVISIONTEMPLATESIMPORTER_H
#define SRC_HOI4WORLD_MILITARY_DIVISIONTEMPLATESIMPORTER_H



#include <filesystem>
#include <vector>

#include "src/hoi4_world/military/division_template.h"



namespace hoi4
{

std::vector<DivisionTemplate> ImportDivisionTemplates(const std::filesystem::path& templates_filename);

}  // namespace hoi4


#endif  // SRC_HOI4WORLD_MILITARY_DIVISIONTEMPLATESIMPORTER_H
