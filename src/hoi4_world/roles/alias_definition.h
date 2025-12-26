#pragma once



#include <string>



namespace hoi4
{

struct AliasDefinition
{
   std::string country_flag;
   std::string alias_identifier;

   auto operator<=>(const AliasDefinition&) const = default;
};

}  // namespace hoi4