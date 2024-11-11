#ifndef SRC_MAPPERS_TECHNOLOGY_TECHMAPPING_H
#define SRC_MAPPERS_TECHNOLOGY_TECHMAPPING_H



#include <optional>
#include <set>
#include <string>



namespace mappers
{

class TechMapping
{
  public:
   TechMapping(std::set<std::string> vic3_requirements, std::optional<std::string> limit, std::set<std::string> techs):
       vic3_requirements_(std::move(vic3_requirements)),
       limit_(std::move(limit)),
       techs_(std::move(techs))
   {
   }

   [[nodiscard]] const std::set<std::string>& GetVic3Requirements() const { return vic3_requirements_; }
   [[nodiscard]] const std::optional<std::string>& GetLimit() const { return limit_; }
   [[nodiscard]] const std::set<std::string>& GetTechs() const { return techs_; }

   std::strong_ordering operator<=>(const TechMapping&) const = default;

  private:
   std::set<std::string> vic3_requirements_;
   std::optional<std::string> limit_;
   std::set<std::string> techs_;
};

}  // namespace mappers



#endif  // SRC_MAPPERS_TECHNOLOGY_TECHMAPPING_H