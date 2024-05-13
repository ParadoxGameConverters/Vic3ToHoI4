#ifndef SRC_HOI4WORLD_ROLES_STORIESCREATOR_H
#define SRC_HOI4WORLD_ROLES_STORIESCREATOR_H


#include <map>
#include <string>

#include "src/hoi4_world/countries/hoi4_country.h"
#include "src/hoi4_world/roles/role.h"



namespace hoi4
{

std::vector<std::pair<std::string, hoi4::Role>> CreateStories(const std::map<std::string, hoi4::Country>& countries);

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_ROLES_STORIESCREATOR_H