#ifndef SRC_HOI4WORLD_ROLES_ROLESIMPORTER_H
#define SRC_HOI4WORLD_ROLES_ROLESIMPORTER_H



#include <map>
#include <string>

#include "role.h"



namespace hoi4
{

std::map<std::string, Role> ImportRoles();

}



#endif  // SRC_HOI4WORLD_ROLES_ROLESIMPORTER_H