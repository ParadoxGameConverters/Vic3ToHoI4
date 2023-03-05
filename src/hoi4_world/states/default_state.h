#ifndef SRC_HOI4WORLD_STATES_DEFAULTSTATES_H
#define SRC_HOI4WORLD_STATES_DEFAULTSTATES_H


#include <map>
#include <set>
#include <string>



namespace hoi4
{

struct DefaultStateOptions
{
   bool impassable = false;
   std::string owner_tag;
   std::set<int> provinces;
   int civilian_factories = 0;
   int military_factories = 0;
   int dockyards = 0;
   std::map<std::string, double> resources;
};


class DefaultState
{
  public:
   explicit DefaultState(DefaultStateOptions options):
       impassable_(options.impassable),
       owner_tag_(std::move(options.owner_tag)),
       provinces_(std::move(options.provinces)),
       civilian_factories_(options.civilian_factories),
       military_factories_(options.military_factories),
       dockyards_(options.dockyards),
       resources_(std::move(options.resources))
   {
   }

   [[nodiscard]] bool IsImpassable() const { return impassable_; }
   [[nodiscard]] const std::string& GetOwner() const { return owner_tag_; }
   [[nodiscard]] const std::set<int>& GetProvinces() const { return provinces_; }
   [[nodiscard]] int GetCivFactories() const { return civilian_factories_; }
   [[nodiscard]] int GetMilFactories() const { return military_factories_; }
   [[nodiscard]] int GetDockyards() const { return dockyards_; }
   [[nodiscard]] const std::map<std::string, double>& GetResources() const { return resources_; }

   std::partial_ordering operator<=>(const DefaultState&) const = default;

  private:
   bool impassable_ = false;
   std::string owner_tag_;
   std::set<int> provinces_;
   int civilian_factories_ = 0;
   int military_factories_ = 0;
   int dockyards_ = 0;
   std::map<std::string, double> resources_;
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_STATES_DEFAULTSTATES_H