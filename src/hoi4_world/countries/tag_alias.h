#pragma once



#include <string>



namespace hoi4
{

struct TagAlias
{
   std::string alias;
   std::string original_tag;
   std::string flag;

   std::strong_ordering operator<=>(const TagAlias&) const = default;
};

}  // namespace hoi4