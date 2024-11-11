#include "converter_utils.h"

#include "external/commonItems/Log.h"
#include "external/fmt/include/fmt/format.h"



void OutputStats(const std::string& stat_name, double converted_value, double base_value)
{
   double percent = static_cast<double>(converted_value - base_value) / static_cast<double>(base_value) * 100.0;
   if (!std::isfinite(percent))
   {
      percent = 0.0;
   }

   Log(LogLevel::Info) << fmt::format("\t\t{} conversion: total={:.3f}, target={:.3f}, diff={:.3f}%",
       stat_name,
       converted_value,
       base_value,
       percent);
}