#ifndef SRC_MAPPERS_CHARACTER_GENERICADVISORMAPPER_H
#define SRC_MAPPERS_CHARACTER_GENERICADVISORMAPPER_H



#include <map>
#include <string>

#include "src/hoi4_world/characters/hoi4_character.h"

namespace hoi4{
	using Trait = std::string
	using Slot = std::string
}

namespace mappers
{

using AdvisorMap = std::map<hoi4::Slot, hoi4::Trait>;

class GenericAdvisorMapper
{
  public:
   explicit GenericAdvisorMapper(AdvisorMap advisor_map):
       advisor_map_(std::move(advisor_map)),
       
   {
   }

   [[nodiscard]] hoi4::Advisor GetGenericAdvisors() const;

  private:
   AdvisorMap advisor_map_;
};

}  // namespace mappers



#endif  // SRC_MAPPERS_CHARACTER_LEADERTYPEMAPPER_H