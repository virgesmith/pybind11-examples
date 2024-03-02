#pragma once

#include <cstdint>

uint64_t fib_recursive(uint64_t n);

class FibGenerator
{
public:
    FibGenerator() : a(0), b(1) {}

    uint64_t next();

    FibGenerator& iter();

private:
    uint64_t a;
    uint64_t b;
};

