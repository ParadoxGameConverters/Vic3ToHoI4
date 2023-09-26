#ifndef SRC_OUTHOI4_FLAGS_OUTFLAGS_H
#define SRC_OUTHOI4_FLAGS_OUTFLAGS_H

#include <map>
#include <string>

#include "external/commonItems/ModLoader/ModFilesystem.h"
#include "src/hoi4_world/countries/hoi4_country.h"


namespace out
{

void OutputFlags(const std::string& output_name,
    const std::map<std::string, hoi4::Country>& countries,
    const commonItems::ModFilesystem& hoi4_mod_filesystem);

}  // namespace out



#endif  // SRC_OUTHOI4_FLAGS_OUTFLAGS_H
