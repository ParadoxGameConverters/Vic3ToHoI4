#ifndef SRC_MAPPERS_UNIT_UNITMAPPERIMPORTER_H
#define SRC_MAPPERS_UNIT_UNITMAPPERIMPORTER_H



#include <filesystem>

#include "src/mappers/unit/unit_mapper.h"



namespace mappers
{

UnitMapper ImportUnitMapper(const std::filesystem::path& mapping_file);

}  // namespace mappers



#endif  // SRC_MAPPERS_UNIT_UNITMAPPERIMPORTER_H
