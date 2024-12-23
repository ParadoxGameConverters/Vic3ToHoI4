#ifndef SRC_VIC3WORLD_MILITARY_MILITARYFORMATIONIMPORTER_H
#define SRC_VIC3WORLD_MILITARY_MILITARYFORMATIONIMPORTER_H



#include <external/commonItems/Parser.h>

#include "src/vic3_world/military/military_formation.h"



namespace vic3
{

class MilitaryFormationImporter
{
  public:
   MilitaryFormationImporter();
   std::optional<MilitaryFormation> ImportMilitaryFormation(std::istream& input);

  private:
   std::optional<int> country_;
   MilitaryFormationType type_;
   std::optional<std::string> name_;
   std::optional<int> ordinal_number_;
   std::map<std::string, int> units_;

   commonItems::parser military_formation_parser_;
   commonItems::parser building_to_expected_unit_parser_;
   commonItems::parser building_parser_;
   commonItems::parser unit_types_num_list_parser_;
};

}  // namespace vic3



#endif  // SRC_VIC3WORLD_MILITARY_MILITARYFORMATIONIMPORTER_H