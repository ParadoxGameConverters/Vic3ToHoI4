#ifndef SRC_SUPPORT_PSEDUORANDOM_PSEDUORANDOMORDER_H
#define SRC_SUPPORT_PSEDUORANDOM_PSEDUORANDOMORDER_H

// Technically, these are the least random permutations possible,
// but to a human eye it'll make an alphabetical list look random

// Will only ever hit a subset of permutations (n * (n - 1)) for primes, less for non-primes.
// But every permutation it does hit will be valid and equally likely to be selected.
// They'll also be sufficiently different from each other.
// Good enough for name-list shuffles.
// Use playthrough-id as the seed and you'll get behavior deterministic to game-save.
std::vector<int> PseduoRandomizeOrder(int seed, const std::vector<int>& container);


#endif  // SRC_SUPPORT_PSEDUORANDOM_PSEDUORANDOMORDER_H