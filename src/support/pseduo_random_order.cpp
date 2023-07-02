#include "src/support/pseduo_random_order.h"

#include <numeric>

namespace
{
std::vector<int> GetCoPrimesLessThan(const int n)
{
   const int start = n < 5 ? 1 : 2;    // All permutation when n < 5
   const int end = n < 5 ? n : n - 1;  // Only mixed permutations when n >= 5

   std::vector<int> co_primes;  // Works for every number in the universe except 4 & 6
   for (int i = start; i < end; ++i)
   {
      if (std::gcd(i, n) == 1)
      {
         co_primes.push_back(i);
      }
   }
   return co_primes;
}

std::pair<int, int> ProcessSeed(const int seed)
{
   constexpr int kODD_BITS = 1431655765;  // All the odd bits in a signed 32-bit integer
   constexpr int kEVEN_BITS = 715827882;  // All the even bits in a signed 32-bit integer

   return {seed & kODD_BITS, (seed & kEVEN_BITS) / 2};
}

std::vector<int> HandleSpecial(const int seed, const int n)
{
   const auto [_, even_bits_seed] = ProcessSeed(seed);
   const int shift = even_bits_seed % n;

   const std::vector<int> pre_shift_order{1, 3, 0, 2, 5, 4};  // Support 4 & 6
   std::vector<int> order;
   for (int i = shift; i < n + shift; ++i)
   {
      order.push_back(pre_shift_order[(i) % n]);
   }
   return order;
}
}  // namespace


std::vector<int> PseduoRandomizeOrder(const int seed, const std::vector<int>& container)
{
   const int clade = static_cast<int>(container.size());
   if (clade == 0)
   {
      return {};
   }
   if (clade == 4)
   {
      return HandleSpecial(seed, 4);
   }
   if (clade == 6)
   {
      return HandleSpecial(seed, 6);
   }

   const std::vector<int> co_primes = GetCoPrimesLessThan(clade);
   const auto [odd_bits_seed, even_bits_seed] = ProcessSeed(seed);

   const int scramble = co_primes.empty() ? 0 : co_primes.at(odd_bits_seed % static_cast<int>(co_primes.size()));
   const int shift = even_bits_seed % clade;

   std::vector<int> order;
   for (int i = shift; i < clade + shift; ++i)
   {
      order.push_back((i * scramble) % clade);
   }
   return order;
}