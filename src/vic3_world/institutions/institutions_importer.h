#pragma once

#include <vector>
#include <istream>

#include "src/vic3_world/institutions/institution.h"


namespace vic3
{
std::vector<Institution> ImportInstitutions(std::istream& input_stream);
}
