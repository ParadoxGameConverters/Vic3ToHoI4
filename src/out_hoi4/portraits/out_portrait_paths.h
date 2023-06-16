#ifndef OUTHOI4_PORTRAITS_OUTPORTRAITPATHS_H
#define OUTHOI4_PORTRAITS_OUTPORTRAITPATHS_H



#include <ostream>

#include "src/mappers/culture/culture_graphics_mapping.h"



namespace mappers
{

std::ostream& operator<<(std::ostream& out, const PortraitPaths& portrait_paths);
std::ostream& operator<<(std::ostream& out, const IdeologyPortraitPaths& ideology_portrait_paths);

}  // namespace mappers



#endif  // OUTHOI4_PORTRAITS_OUTPORTRAITPATHS_H