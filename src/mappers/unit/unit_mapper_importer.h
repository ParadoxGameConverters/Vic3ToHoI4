#ifndef SRC_MAPPERS_UNIT_UNITMAPPERIMPORTER_H
#define SRC_MAPPERS_UNIT_UNITMAPPERIMPORTER_H


#include "src/mappers/unit/unit_mapper.h"


namespace mappers
{

UnitMapper ImportUnitMapper(std::string_view mapping_file);

}  // namespace mappers



#endif  // SRC_MAPPERS_UNIT_UNITMAPPERIMPORTER_H
