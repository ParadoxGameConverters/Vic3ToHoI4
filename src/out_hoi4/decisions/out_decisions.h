#ifndef SRC_OUTHOI4_DECISIONS_OUTDECISIONS_H
#define SRC_OUTHOI4_DECISIONS_OUTDECISIONS_H



#include <filesystem>
#include <map>
#include <string_view>

#include "src/hoi4_world/countries/hoi4_country.h"



namespace out
{

void OutputDecisions(const std::filesystem::path& output_name, const std::map<std::string, hoi4::Country>& countries);

}  // namespace out



#endif  // SRC_OUTHOI4_DECISIONS_OUTDECISIONS_H
