#ifndef SRC_OUTHOI4_DECISIONS_OUTDECISIONS_H
#define SRC_OUTHOI4_DECISIONS_OUTDECISIONS_H

#include <map>
#include <string_view>

#include "src/hoi4_world/countries/hoi4_country.h"


namespace out
{

void OutputDecisions(std::string_view output_name, const std::map<std::string, hoi4::Country>& countries);

}  // namespace out


#endif  // SRC_OUTHOI4_DECISIONS_OUTDECISIONS_H
