#include "src/out_hoi4/military/out_equipment_variant.h"

#include "external/fmt/include/fmt/format.h"



std::ostream& out::operator<<(std::ostream& out_stream, const hoi4::EquipmentVariant& equipment_variant)
{
   out_stream << "\tcreate_equipment_variant = {\n";
   for (const auto& [key, value]: equipment_variant.GetTextItems())
   {
      out_stream << fmt::format("\t\t{} {}\n", key, value);
   }
   out_stream << "\t}\n";

   return out_stream;
}