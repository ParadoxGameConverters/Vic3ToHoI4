#include "src/vic3_world/provinces/vic3_province_definitions.h"



std::optional<std::string> vic3::ProvinceDefinitions::GetProvinceDefinition(int province_number) const
{
   if (const auto definition = province_definitions_.find(province_number); definition != province_definitions_.end())
   {
      return definition->second;
   }

   return std::nullopt;
}