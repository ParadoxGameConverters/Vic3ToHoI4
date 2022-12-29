#include "src/vic3_world/provinces/vic3_province_definitions.h"



std::optional<std::string> vic3::ProvinceDefinitions::GetProvinceDefinition(int province_number) const
{
   if (province_definitions_.size() < static_cast<unsigned int>(province_number))
   {
      return std::nullopt;
   }

   return province_definitions_.at(province_number - 1);
}