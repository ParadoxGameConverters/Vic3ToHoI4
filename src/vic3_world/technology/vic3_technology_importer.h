#ifndef SRC_VIC3WORLD_TECHNOLOGY_VIC3TECHNOLOGYIMPORTER_H
#define SRC_VIC3WORLD_TECHNOLOGY_VIC3TECHNOLOGYIMPORTER_H



#include <istream>
#include <map>
#include <set>
#include <vector>



namespace vic3
{

std::map<int, std::vector<std::string>> ImportAcquiredTechnologies(std::istream& input_stream);

}  // namespace vic3



#endif  // SRC_VIC3WORLD_TECHNOLOGY_VIC3TECHNOLOGYIMPORTER_H