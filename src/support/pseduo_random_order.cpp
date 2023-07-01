#include "src/support/pseduo_random_order.h"

#include <numeric>

namespace
{
std::vector<int> GetCoPrimesLessThan(const int n)
{
   std::vector<int> co_primes;
   for (int i = 1; i < n; ++i)
   {
      if (std::gcd(i, n) == 1)
      {
         co_primes.push_back(i);
      }
   }
   return co_primes;
   /*
    Alternate version that prunes all in-order permutations (1,2,3,4,5) or (5,4,3,2,1) or (3,4,5,1,2).
    Works for every positive integer in the universe except 6.
    
    std::vector<int> co_primes;
	for (int i = 2; i < n - 1; ++i)
	{
		if (std::gcd(i, n) == 1)
	    {
			co_primes.push_back(i);
	    }
	}
	return co_primes;
    */
}

std::pair<int, int> ProcessSeed(const int seed)
{
   constexpr int kODD_BITS = 1431655765;  // All the odd bits in a signed 32-bit integer
   constexpr int kEVEN_BITS = 715827882;  // All the even bits in a signed 32-bit integer

   return {seed & kODD_BITS, (seed & kEVEN_BITS) / 2};
}
}  // namespace

#pragma optimize("", off)
std::vector<int> PseduoRandomizeOrder(const int seed, const std::vector<int>& container)
{
   const int clade = static_cast<int>(container.size());
   if (clade == 0)
   {
      return {};
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
#pragma optimize("", on)