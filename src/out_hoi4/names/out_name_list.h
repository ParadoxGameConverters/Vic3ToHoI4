#ifndef OUTHOI4_NAMES_OUTNAMELIST_H
#define OUTHOI4_NAMES_OUTNAMELIST_H



#include <ostream>

#include "src/hoi4_world/countries/hoi4_country.h"



namespace hoi4
{

std::ostream& operator<<(std::ostream& out, const NameList& name_list);

}  // namespace hoi4



#endif  // OUTHOI4_NAMES_OUTNAMELIST_H