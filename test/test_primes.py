from functools import reduce
from operator import mul
import random
import pytest

from pybind11_examples import (
    is_prime,
    nth_prime,
    prime_factors,
    PrimeSieve,
    PrimeRange,
    PrimeGenerator,
)


def primes(gen):
    yield 2
    yield 3
    p = gen()
    yield from p


def test_sieve():
    p = list(PrimeSieve(100))
    assert p == [
        2,
        3,
        5,
        7,
        11,
        13,
        17,
        19,
        23,
        29,
        31,
        37,
        41,
        43,
        47,
        53,
        59,
        61,
        67,
        71,
        73,
        79,
        83,
        89,
        97,
    ]
    *_, p = list(PrimeSieve(100_001_000))
    assert p == 100_000_969


def test_prime_generator():
    p = primes(PrimeGenerator)
    assert next(p) == 2
    assert next(p) == 3
    assert next(p) == 5
    assert next(p) == 7
    *_, p1000 = (next(p) for _ in range(996))
    assert p1000 == 7919
    *_, p10000 = (next(p) for _ in range(9000))
    assert p10000 == 104729
    *_, p100000 = (next(p) for _ in range(90000))
    assert p100000 == 1299709


def test_nth_prime():
    assert nth_prime(1_000) == 7919
    assert nth_prime(10_000) == 104729
    assert nth_prime(100_000) == 1299709
    assert nth_prime(1_000_000) == 15485863


def test_is_prime():
    assert is_prime(997)
    assert not is_prime(998)
    assert not is_prime(1007)
    assert is_prime(1009)
    assert is_prime(7919)
    assert is_prime(104729)
    assert is_prime(1299709)
    assert not is_prime(2**30 - 1)
    assert is_prime(2**31 - 1)

    p = list(PrimeRange(3, 1000))
    for n in range(3, 1000):
        assert not (n in p) ^ is_prime(n)

    p = list(PrimeRange(1000000, 1001000))
    for n in range(1000000, 1001000):
        assert not (n in p) ^ is_prime(n)


def test_prime_factors():
    with pytest.raises(ValueError):
        prime_factors(0)
    assert prime_factors(1) == []
    assert prime_factors(2) == [2]
    assert prime_factors(3) == [3]
    assert prime_factors(4) == [2, 2]
    assert prime_factors(5) == [5]
    assert prime_factors(6) == [2, 3]

    random.seed(19937)
    numbers = random.choices(range(1000000), k=100)
    for n in numbers:
        assert reduce(mul, prime_factors(n)) == n

    numbers = random.choices(range(100000000, 101000000), k=10)
    for n in numbers:
        assert reduce(mul, prime_factors(n)) == n
