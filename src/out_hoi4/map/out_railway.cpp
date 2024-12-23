#include "src/out_hoi4/map/out_railway.h"

#include <external/fmt/include/fmt/format.h>



std::ostream& hoi4::operator<<(std::ostream& out, const Railway& railway)
{
   out << fmt::format("{} {}", railway.GetLevel(), railway.GetLength());
   for (const int province: railway.GetProvinces())
   {
      out << fmt::format(" {}", province);
   }
   out << "\n";

   return out;
}