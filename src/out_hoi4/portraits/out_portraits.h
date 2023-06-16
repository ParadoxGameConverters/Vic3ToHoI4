#ifndef OUTHOI4_PORTRAITS_OUTPORTRAITS_H
#define OUTHOI4_PORTRAITS_OUTPORTRAITS_H



#include <vector>

#include "src/hoi4_world/countries/hoi4_country.h"



namespace out
{

void OutputPortraits(std::string_view output_name, const std::map<std::string, hoi4::Country>& countries);

}  // namespace out



#endif  // OUTHOI4_PORTRAITS_OUTPORTRAITS_H