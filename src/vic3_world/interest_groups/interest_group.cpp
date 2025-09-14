#include "src/vic3_world/interest_groups/interest_group.h"



namespace vic3
{

bool InterestGroup::operator==(const InterestGroup& other) const
{
   if (type_ != other.type_)
   {
      return false;
   }
   if (country_id_.Get() != other.country_id_.Get())
   {
      return false;
   }
   if (leader_.Get() != other.leader_.Get())
   {
      return false;
   }
   if (clout_.Get() != other.clout_.Get())
   {
      return false;
   }
   if (in_government_.Get() != other.in_government_.Get())
   {
      return false;
   }
   if (ideologies_ != other.ideologies_)
   {
      return false;
   }
   return true;
}

}  // namespace vic3