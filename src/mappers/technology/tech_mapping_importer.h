#ifndef SRC_MAPPERS_TECHNOLOGY_TECHMAPPINGIMPORTER_H
#define SRC_MAPPERS_TECHNOLOGY_TECHMAPPINGIMPORTER_H



#include <external/commonItems/Parser.h>

#include <set>
#include <string>

#include "src/mappers/technology/tech_mapping.h"



namespace mappers
{

class TechMappingImporter
{
  public:
   TechMappingImporter();

   TechMapping ImportTechMapping(std::istream& input_stream);

  private:
   commonItems::parser parser;

   std::set<std::string> vic3_requirements_;
   std::optional<std::string> limit_;
   std::set<std::string> techs_;
};

}  // namespace mappers



#endif  // SRC_MAPPERS_TECHNOLOGY_TECHMAPPINGIMPORTER_H