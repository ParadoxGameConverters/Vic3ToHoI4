#ifndef OUTHOI4_NAMES_OUTNAMES_H
#define OUTHOI4_NAMES_OUTNAMES_H



#include <vector>

#include "src/hoi4_world/countries/hoi4_country.h"



namespace out
{

void OutputNames(std::string_view output_name, const std::map<std::string, hoi4::Country>& countries);

}  // namespace out



#endif  // OUTHOI4_NAMES_OUTNAMES_H