#ifndef SRC_HOI4WORLD_ROLES_H
#define SRC_HOI4WORLD_ROLES_H



#include <string>
#include <vector>



namespace hoi4
{

struct RoleOptions
{
   std::string name;
   std::string category;
   std::string requirements;
   float score;
   std::vector<std::string> blockers;
   std::vector<std::string> focuses;
   std::vector<std::string> repeat_focuses;
   std::vector<std::string> removed_focuses;
   std::vector<std::string> decisions;
   std::vector<std::string> events;
};


class Role
{
  public:
   Role(RoleOptions options):
       name_(std::move(options.name)),
       category_(std::move(options.category)),
       requirements_(std::move(options.requirements)),
       score_(options.score),
       blockers_(std::move(options.blockers)),
       focuses_(std::move(options.focuses)),
       repeat_focuses_(std::move(options.repeat_focuses)),
       removed_focuses_(std::move(options.removed_focuses)),
       decisions_(std::move(options.decisions)),
       events_(std::move(options.events))
   {
   }

  private:
   std::string name_;
   std::string category_;
   std::string requirements_;
   float score_;
   std::vector<std::string> blockers_;
   std::vector<std::string> focuses_;
   std::vector<std::string> repeat_focuses_;
   std::vector<std::string> removed_focuses_;
   std::vector<std::string> decisions_;
   std::vector<std::string> events_;
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_ROLES_H