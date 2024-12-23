#ifndef SRC_MAPPERS_PROVINCE_PROVINCEMAPPING_IMPORTER_H
#define SRC_MAPPERS_PROVINCE_PROVINCEMAPPING_IMPORTER_H



#include <external/commonItems/Parser.h>

#include <optional>
#include <string>
#include <vector>

#include "src/mappers/provinces/province_mapping.h"



namespace mappers
{

class ProvinceMappingImporter
{
  public:
   ProvinceMappingImporter();
   ProvinceMapping ImportProvinceMapping(std::istream& input_stream);

  private:
   commonItems::parser parser_;

   std::vector<std::string> vic3_provinces_;
   std::vector<int> hoi4_provinces_;
   std::optional<std::string> comment_;
};

}  // namespace mappers



#endif  // SRC_MAPPERS_PROVINCE_PROVINCEMAPPING_IMPORTER_H