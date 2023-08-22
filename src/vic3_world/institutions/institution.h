#pragma once

namespace vic3
{
struct Institution
{
   std::string type;
   int country = 0;
   int investment = 0;
   auto operator<=>(Institution const& other) const = default;
   //bool operator==(Institution const& other) const = default;
};
}  // namespace vic3