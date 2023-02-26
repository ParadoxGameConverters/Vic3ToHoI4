#ifndef SRC_HOI4WORLD_TECHNOLOGY_TECHNOLOGIESCONVERTER_H_
#define SRC_HOI4WORLD_TECHNOLOGY_TECHNOLOGIESCONVERTER_H_



#include <vector>

#include "src/hoi4_world/technology/technologies.h"
#include "src/mappers/technology/tech_mapping.h"



namespace hoi4
{

Technologies ConvertTechnologies(const std::set<std::string>& vic3_technologies,
    const std::vector<mappers::TechMapping>& tech_mappings);

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_TECHNOLOGY_TECHNOLOGIESCONVERTER_H_