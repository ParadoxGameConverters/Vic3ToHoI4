#pragma once


constexpr int FloorMod(const int lhs, const int rhs)
{
   return (lhs % rhs + rhs) % rhs;
}

void OutputStats(const std::string& stat_name, double converted_value, double base_value);