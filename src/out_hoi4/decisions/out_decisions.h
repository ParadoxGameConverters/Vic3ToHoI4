#ifndef SRC_OUTHOI4_DECISIONS_OUTDECISIONS_H
#define SRC_OUTHOI4_DECISIONS_OUTDECISIONS_H



#include <filesystem>
#include <map>
#include <vector>

#include "src/hoi4_world/decisions/decision.h"



namespace out
{

void OutputDecisions(const std::filesystem::path& output_name,
    const std::map<std::string, std::vector<hoi4::Decision>>& decisions);

}  // namespace out



#endif  // SRC_OUTHOI4_DECISIONS_OUTDECISIONS_H
