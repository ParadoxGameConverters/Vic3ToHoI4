#ifndef SRC_SUPPORT_CONVERTERUTILS_H_
#define SRC_SUPPORT_CONVERTERUTILS_H_



#include <string>



constexpr int FloorMod(const int lhs, const int rhs)
{
   return (lhs % rhs + rhs) % rhs;
}


void OutputStats(const std::string& stat_name, double converted_value, double base_value);



#endif  // SRC_SUPPORT_CONVERTERUTILS_H_