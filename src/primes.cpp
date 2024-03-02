#include "primes.h"

#include <pybind11/pybind11.h>

#include <cmath>

namespace py = pybind11;

// #include <stdexcept>
// namespace py {
//     using value_error = std::runtime_error;
//     using stop_iteration = std::runtime_error;
// }

namespace {

size_t isqrt(size_t n) {
    return static_cast<size_t>(std::sqrt(n)) + 1;
}

bool is_prime(size_t n, const std::vector<size_t>& primes_below) noexcept {
    switch (n) {
        case 0:
        case 1:
            return false;
        case 2:
        case 3:
            return true;
        default:
            for (size_t p: primes_below) {
                if (n % p == 0) {
                    return false;
                }
                // only need to go as far as sqrt(n)
                if (p * p > n) {
                    break;
                }
            }
            return true;
    }
}


// O(n) memory required
std::vector<size_t> sieve0(size_t n) {
    n = std::max(n, (size_t)4);
    std::vector<bool> state(n, true);
    state[0] = state[1] = false;
    size_t m = isqrt(n);
    for (size_t i = 2; i < m; ++i) {
        if (state[i]) {
            for (size_t j = i * i; j < n; j += i) {
                state[j] = false;
            }
        }
    }
    // TODO more functional approach? cf state.iter().enumerate().filter(|(_, &s)| s).map(|(i, _)| i).collect::<Vec<_>>()
    std::vector<size_t> primes;
    for (size_t i = 0; i < n; ++i) {
        if (state[i]) {
            primes.push_back(i);
        }
    }
    return primes;
}


std::vector<size_t> sieve(size_t n) {
    n = std::max(n, (size_t)4);
    const size_t chunk_size = std::min(n, (size_t)100'000'000);
    auto primes = sieve0(chunk_size);

    for (size_t n0 = chunk_size; n0 < n; n0 += chunk_size) {
        size_t n1 = std::min(n0 + chunk_size, n);
        size_t m = isqrt(n1);
        std::vector<bool> state(n1 - n0, true);
        for (size_t p: primes) {
            if (p > m) {
                break;
            }
            size_t s = n0 % p ? p * (n0 / p + 1) : p * (n0 / p);
            for (size_t i = s; i < n1; i += p) {
                state[i - n0] = false;
            }
        }
        for (size_t i = 0; i < state.size(); ++i) {
            if (state[i]) {
                primes.push_back(n0 + i);
            }
        }
    }
    return primes;
}

}

// TODO? sieve to sqrt(n) then check primality in next
PrimeSieve::PrimeSieve(size_t n) : index{0}, primes(sieve(n)) {
}


PrimeSieve& PrimeSieve::iter() {
    return *this;
}


size_t PrimeSieve::next() {
    if (index == primes.size()) {
        throw py::stop_iteration();
    }
    return primes[index++];
}


PrimeGenerator::PrimeGenerator() : found{2, 3} {
}


PrimeGenerator& PrimeGenerator::iter() {
    return *this;
}


size_t PrimeGenerator::next() {
    size_t n = found.back();
    for (;;) {
        n += 2;
        if (is_prime(n, found)) {
            found.push_back(n);
            return n;
        }
    }
}


PrimeRange::PrimeRange(size_t m, size_t n): index{m % 2 ? m : m + 1}, n(n), m_seed_primes(sieve(isqrt(n))) {
}

PrimeRange& PrimeRange::iter() {
    return *this;
}

size_t PrimeRange::next() {
    while (index <= n && !is_prime(index, m_seed_primes)) {
        index += 2;
    }
    size_t ret = index;
    index += 2;
    if (ret > n)
        throw py::stop_iteration();
    return ret;
}


bool is_prime_py(size_t n) {
    return is_prime(n, sieve(std::max(isqrt(n), (size_t)10)));
}


size_t nth_prime(size_t n) noexcept {
    if (n < 6)
        return std::vector<size_t>{2, 3, 5, 7, 11, 13}[n-1];
    // https://stackoverflow.com/questions/1042717/is-there-a-way-to-find-the-approximate-value-of-the-nth-prime
    size_t m = n < 7022 ? n * (log(n) + log(log(n))) : n * (log(n) + (log(log(n)) - 0.9385));
    const auto& p = sieve(m);
    return p[n-1];
}


size_t nth_prime_py(size_t n) {
    if (n == 0) {
        throw py::value_error("n must be >0");
    }
    return nth_prime(n);
}


std::vector<size_t> prime_factors(size_t n) {
    if (n == 0) {
        throw py::value_error("input must be >=1");
    }
    std::vector<size_t> factors;
    for (size_t p: sieve(isqrt(n))) {
        while (n % p == 0) {
            n /= p;
            factors.push_back(p);
        }
    }
    if (n > 1) {
        factors.push_back(n);
    }
    return factors;
}
