#include "src/hoi4_world/roles/triggers/any_primary_culture_trigger.h"

#include <algorithm>
#include <ranges>

#include "src/vic3_world/cultures/culture_definition.h"



namespace hoi4
{

bool AnyPrimaryCultureTrigger::IsValid(const Context& context, const World& world) const
{
   const CountryScope* maybe_country = std::get_if<CountryScope>(&context.this_scope);
   if (!maybe_country)
   {
      return false;
   }
   const Country& country = maybe_country->country;

   const auto culture_is_valid = [context, country, &world, this](const vic3::CultureDefinition& culture) {
      return std::ranges::all_of(children_.begin(),
          children_.end(),
          [context, country, &world, &culture](const std::unique_ptr<Trigger>& a) {
             const Context new_context{
                 .root = context.root,
                 .this_scope = CultureScope{.culture = culture},
                 .prev = context.this_scope,
                 .from = context.from,
             };
             return a->IsValid(new_context, world);
          });
   };

   std::vector<vic3::CultureDefinition> primary_cultures;
   for (const std::string& culture_name: country.GetPrimaryCultures())
   {
      const std::map<std::string, vic3::CultureDefinition>& culture_definitions = world.GetCultureDefinitions();
      const auto culture = culture_definitions.find(culture_name);
      if (culture == culture_definitions.end())
      {
         continue;
      }
      primary_cultures.emplace_back(culture->second);
   }

   return std::ranges::any_of(primary_cultures, culture_is_valid);
}


std::vector<Scope> AnyPrimaryCultureTrigger::FindAllValid(const Context& context, const World& world) const
{
   std::vector<Scope> valid_scopes;

   const CountryScope* maybe_country = std::get_if<CountryScope>(&context.this_scope);
   if (!maybe_country)
   {
      return {};
   }
   const Country& country = maybe_country->country;

   const auto culture_is_valid = [context, country, &world, this](const vic3::CultureDefinition& culture) {
      return std::ranges::all_of(children_.begin(),
          children_.end(),
          [context, &world, &culture](const std::unique_ptr<Trigger>& a) {
             const Context new_context{
                 .root = context.root,
                 .this_scope = CultureScope{.culture = culture},
                 .prev = context.this_scope,
                 .from = context.from,
             };
             return a->IsValid(new_context, world);
          });
   };

   for (const std::string& culture_name: country.GetPrimaryCultures())
   {
      const std::map<std::string, vic3::CultureDefinition>& culture_definitions = world.GetCultureDefinitions();
      const auto culture = culture_definitions.find(culture_name);
      if (culture == culture_definitions.end())
      {
         continue;
      }
      if (std::ranges::all_of(children_.begin(),
              children_.end(),
              [context, country, &world, &culture](const std::unique_ptr<Trigger>& a) {
                 const Context new_context{
                     .root = context.root,
                     .this_scope = CultureScope{.culture = culture->second},
                     .prev = context.this_scope,
                     .from = context.from,
                 };
                 return a->IsValid(new_context, world);
              }))
      {
         valid_scopes.emplace_back(CultureScope{.culture = culture->second});
      }
   }

   return valid_scopes;
}


bool AnyPrimaryCultureTrigger::operator==(const AnyPrimaryCultureTrigger& rhs) const
{
   if (children_.size() != rhs.children_.size())
   {
      return false;
   }

   for (unsigned i = 0; i < children_.size(); ++i)
   {
      if (*children_[i] != *rhs.children_[i])
      {
         return false;
      }
   }

   return true;
}


bool AnyPrimaryCultureTrigger::operator<(const AnyPrimaryCultureTrigger& rhs) const
{
   if (children_.size() < rhs.children_.size())
   {
      return true;
   }
   if (children_.size() > rhs.children_.size())
   {
      return false;
   }

   for (unsigned i = 0; i < children_.size(); ++i)
   {
      if (*children_[i] < *rhs.children_[i])
      {
         return true;
      }
      if (*children_[i] != *rhs.children_[i])
      {
         return false;
      }
   }

   return true;
}


std::unique_ptr<Trigger> AnyPrimaryCultureTrigger::Copy() const
{
   std::vector<std::unique_ptr<Trigger>> children;
   for (const auto& child: children_)
   {
      children.push_back(child->Copy());
   }

   return std::make_unique<AnyPrimaryCultureTrigger>(std::move(children));
}


void PrintTo(const AnyPrimaryCultureTrigger& trigger, std::ostream* os)
{
   *os << "AnyPrimaryCultureTrigger: {\n";
   for (const std::unique_ptr<Trigger>& child: trigger.children_)
   {
      *os << child;
   }
   *os << "}\n";
}

}  // namespace hoi4