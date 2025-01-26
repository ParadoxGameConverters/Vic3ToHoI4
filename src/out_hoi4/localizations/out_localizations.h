#ifndef SRC_OUTHOI4_LOCALIZATIONS_OUTLOCALIZATIONS_H
#define SRC_OUTHOI4_LOCALIZATIONS_OUTLOCALIZATIONS_H



#include <filesystem>
#include <string_view>

#include "src/hoi4_world/localizations/localizations.h"



namespace out
{

void OutputLocalizations(const std::filesystem::path& output_name, const hoi4::Localizations& localizations);

}  // namespace out



#endif  // SRC_OUTHOI4_LOCALIZATIONS_OUTLOCALIZATIONS_H