#include "src/hoi4_world/countries/hoi4_country_converter.h"

#include "src/hoi4_world/technology/technologies_converter.h"



namespace
{

std::vector<hoi4::EquipmentVariant> DetermineActiveVariants(const std::vector<hoi4::EquipmentVariant>& all_variants,
    const hoi4::Technologies& technologies)
{
   std::vector<hoi4::EquipmentVariant> active_variants;

   for (const hoi4::EquipmentVariant& variant: all_variants)
   {
      if (!std::ranges::all_of(variant.GetRequiredTechs(), [technologies](const std::string& required_technology) {
             return technologies.HasTechnology(required_technology);
          }))
      {
         continue;
      }
      if (std::ranges::any_of(variant.GetBlockingTechs(), [technologies](const std::string& blocking_technology) {
             return technologies.HasTechnology(blocking_technology);
          }))
      {
         continue;
      }

      active_variants.push_back(variant);
   }

   return active_variants;
}

}  // namespace



std::optional<hoi4::Country> hoi4::ConvertCountry(const vic3::Country& source_country,
    const std::set<std::string>& source_technologies,
    const mappers::CountryMapper& country_mapper,
    const std::map<int, int>& vic3_state_ids_to_hoi4_state_ids,
    const std::vector<mappers::TechMapping>& tech_mappings,
    const std::vector<EquipmentVariant>& all_legacy_ship_variants,
    const std::vector<EquipmentVariant>& all_ship_variants,
    const std::vector<EquipmentVariant>& all_plane_variants,
    const std::vector<EquipmentVariant>& all_tank_variants)
{
   const auto tag = country_mapper.GetHoiTag(source_country.GetTag());
   if (!tag.has_value())
   {
      return std::nullopt;
   }

   std::optional<int> capital_state;
   if (const std::optional<int> vic3_capital_state = source_country.GetCapitalState(); vic3_capital_state)
   {
      if (const auto state_id_mapping = vic3_state_ids_to_hoi4_state_ids.find(*vic3_capital_state);
          state_id_mapping != vic3_state_ids_to_hoi4_state_ids.end())
      {
         capital_state = state_id_mapping->second;
      }
   }

   const Technologies technologies = ConvertTechnologies(source_technologies, tech_mappings);

   const std::vector<EquipmentVariant>& active_legacy_ship_variants =
       DetermineActiveVariants(all_legacy_ship_variants, technologies);
   const std::vector<EquipmentVariant>& active_ship_variants = DetermineActiveVariants(all_ship_variants, technologies);
   const std::vector<EquipmentVariant>& active_plane_variants =
       DetermineActiveVariants(all_plane_variants, technologies);
   const std::vector<EquipmentVariant>& active_tank_variants = DetermineActiveVariants(all_tank_variants, technologies);

   return Country({.tag = *tag,
       .color = source_country.GetColor(),
       .capital_state = capital_state,
       .technologies = technologies,
       .legacy_ship_variants = active_legacy_ship_variants,
       .ship_variants = active_ship_variants,
       .plane_variants = active_plane_variants,
       .tank_variants = active_tank_variants});
}