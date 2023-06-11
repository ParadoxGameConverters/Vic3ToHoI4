#ifndef OUTHOI4_MAP_OUTNAMES_H
#define OUTHOI4_MAP_OUTNAMES_H



#include <vector>

#include "src/hoi4_world/countries/hoi4_country.h"



namespace out
{

void OutputNames(std::string_view output_name, const std::vector<hoi4::Country>& countries);

}  // namespace out



#endif  // OUTHOI4_MAP_OUTNAMES_H