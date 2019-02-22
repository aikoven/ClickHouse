#pragma once

#include <Core/Types.h>
#include <vector>
#include <random>


namespace DB
{

/// Bloom filter for strings.
class StringBloomFilter
{
public:
    using StorageType = UInt8;
    using Container = std::vector<StorageType>;

    /// size -- size of filter in bytes.
    /// hashes -- number of used hash functions.
    /// seed -- random seed for hash functions generation.
    StringBloomFilter(size_t size_, size_t hashes_, size_t seed_);

    bool find(const char * data, size_t len);
    void add(const char * data, size_t len);
    void clear();

    /// Checks if this contains everything from another bloom filter.
    /// Bloom filters must have equal size and seed.
    bool contains(const StringBloomFilter & bf);

    void merge(const StringBloomFilter & bf);

    const Container & getFilter() const { return filter; };
    void setFilter(Container && new_filter) { filter = std::move(new_filter); };

    /// For debug.
    UInt64 getFingerPrint() const;

    friend bool operator== (const StringBloomFilter & a, const StringBloomFilter & b);
private:
    /// LinearCongruentialGenerator for generating random seeds for hash functions.
    /// It is used here because it is very fast and lightweight.
    using LinearCongruentialGenerator = std::linear_congruential_engine<UInt64, 84589, 45989, 217728>;

    size_t size;
    size_t hashes;
    size_t seed;
    Container filter;
};


bool operator== (const StringBloomFilter & a, const StringBloomFilter & b);

}
