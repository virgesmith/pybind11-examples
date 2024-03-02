#include <vector>
#include <cstddef>


class PrimeGenerator
{
public:
    PrimeGenerator();

    PrimeGenerator& iter();

    size_t next();
private:
    std::vector<size_t> found;

};

class PrimeSieve
{
public:
    PrimeSieve(size_t n);

    PrimeSieve& iter();

    size_t next();
private:
    size_t index;
    std::vector<size_t> primes;
};


class PrimeRange
{
public:
    PrimeRange(size_t m, size_t n);

    PrimeRange& iter();

    size_t next();

private:
    size_t index;
    const size_t n;
    const std::vector<size_t> m_seed_primes;
};


bool is_prime_py(size_t n);

size_t nth_prime_py(size_t n);

std::vector<size_t> prime_factors(size_t n);



