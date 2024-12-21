#ifndef SRC_HOI4WORLD_DECISIONS_DECISIONSCATEGORY_H
#define SRC_HOI4WORLD_DECISIONS_DECISIONSCATEGORY_H



#include <optional>
#include <string>
#include <vector>



namespace hoi4
{

struct DecisionsCategory
{
   std::string name;
   std::string icon;
   std::string picture;
   std::optional<int> priority;
   std::string allowed;
   std::string visible;
   std::string visibility_type;
   std::vector<std::pair<std::string, std::string>> extra_items;

   std::strong_ordering operator<=>(const DecisionsCategory& rhs) const = default;
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_DECISIONS_DECISIONSCATEGORY_H