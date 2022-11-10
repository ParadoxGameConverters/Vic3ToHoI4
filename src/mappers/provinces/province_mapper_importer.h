#ifndef SRC_MAPPERS_PROVINCEMAPPERIMPORTER_H
#define SRC_MAPPERS_PROVINCEMAPPERIMPORTER_H



#include <map>
#include <set>
#include <vector>

#include "external/commonItems/ModLoader/ModFilesystem.h"
#include "external/commonItems/Parser.h"
#include "src/configuration/configuration.h"
#include "src/mappers/provinces/province_mapper.h"
#include "src/mappers/provinces/province_mapping_importer.h"
#include "src/mappers/provinces/province_mapping_types.h"



namespace mappers
{

class ProvinceMapperImporter
{
  public:
   ProvinceMapperImporter(const commonItems::ModFilesystem& filesystem);

   ProvinceMapper ImportProvinceMappings();

  private:
   commonItems::parser version_parser_;
   commonItems::parser mapping_parser_;
   ProvinceMappingImporter mapping_importer_;

   Vic3ToHoi4ProvinceMapping vic3_to_hoi4_province_map_;
   Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_map_;
};


}  // namespace mappers



#endif  // SRC_MAPPERS_PROVINCEMAPPERIMPORTER_H