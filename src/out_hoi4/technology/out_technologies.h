#ifndef SRC_OUTHOI4_TECHNOLOGY_OUTTECHNOLOGIES_H
#define SRC_OUTHOI4_TECHNOLOGY_OUTTECHNOLOGIES_H



#include <ostream>

#include "src/hoi4_world/technology/technologies.h"



namespace out
{

std::ostream& operator<<(std::ostream& out, const hoi4::Technologies& all_technologies);

}  // namespace out



#endif  // SRC_OUTHOI4_TECHNOLOGY_OUTTECHNOLOGY_H