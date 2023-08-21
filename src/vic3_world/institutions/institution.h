#pragma once

namespace vic3
{
struct Institution
{
   std::string type;
   int country;
   int investment;
   auto operator<=>(Institution const& other) const = default;
   bool operator==(Institution const& other) const = default;
};
}  // namespace vic3