#ifndef SRC_HOI4WORLD_ROLES_STORIESCREATOR_H
#define SRC_HOI4WORLD_ROLES_STORIESCREATOR_H


#include <map>
#include <string>

#include "src/hoi4_world/countries/hoi4_country.h"



namespace hoi4
{

void CreateStories(const std::map<std::string, hoi4::Country>& countries);

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_ROLES_STORIESCREATOR_H