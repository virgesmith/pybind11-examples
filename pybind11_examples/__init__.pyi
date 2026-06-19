"""

Pybind11 example plugin
-----------------------
.. currentmodule:: _pybind11_examples
.. autosummary::
    :toctree: _generate

"""

from __future__ import annotations

import collections.abc
import enum
import typing

import numpy
import numpy.typing

_P = typing.ParamSpec("_P")
_R = typing.TypeVar("_R")

__all__: list[str] = [
    "CEnum",
    "Circle",
    "Collatz",
    "CppEnum",
    "FibGenerator",
    "ManagedThing",
    "ONE",
    "PrimeGenerator",
    "PrimeRange",
    "PrimeSieve",
    "Registry",
    "Shape",
    "TWO",
    "Triangle",
    "average_exectime",
    "call_shape",
    "daxpy",
    "exectime",
    "fib_recursive",
    "is_prime",
    "nth_prime",
    "prime_factors",
]

class CEnum(enum.Enum):
    ONE: typing.ClassVar[CEnum]  # value = <CEnum.ONE: 1>
    TWO: typing.ClassVar[CEnum]  # value = <CEnum.TWO: 2>

class CppEnum(enum.Enum):
    FOUR: typing.ClassVar[CppEnum]  # value = <CppEnum.FOUR: 4>
    ONE: typing.ClassVar[CppEnum]  # value = <CppEnum.ONE: 1>
    THREE: typing.ClassVar[CppEnum]  # value = <CppEnum.THREE: 3>
    TWO: typing.ClassVar[CppEnum]  # value = <CppEnum.TWO: 2>

class Shape:
    """

    Abstract base class for shapes. Defines two pure virtual functions, one virtual and one nonvirtual.

    """
    def __init__(self) -> None: ...
    def area(self) -> float: ...
    def colour(self) -> str: ...
    def dim(self) -> int: ...
    def perimeter(self) -> float: ...

class Circle(Shape):
    def __init__(self, radius: typing.SupportsFloat | typing.SupportsIndex) -> None: ...

class Collatz:
    """

    C++ implementation of a Collatz sequence generator.

    """
    def __init__(self, n: typing.SupportsInt | typing.SupportsIndex) -> None: ...
    def __iter__(self) -> Collatz:
        """return iter"""
    def __next__(self) -> int:
        """return next item"""
    def close(self) -> None:
        """generator close"""
    @typing.overload
    def send(self, arg0: typing.SupportsInt | typing.SupportsIndex) -> int:
        """generator send"""
    @typing.overload
    def send(self) -> int:
        """generator send (equivalent to next()"""
    @typing.overload
    def throw(self, type: type, value: str = "", traceback: typing.Any = None) -> None:
        """generator throw"""
    @typing.overload
    def throw(self) -> None:
        """generator throw default"""

class FibGenerator:
    """

    C++ implementation of a Fibonacci sequence generator.

    """
    def __init__(self) -> None: ...
    def __iter__(self) -> FibGenerator:
        """__iter__ dunder"""
    def __next__(self) -> int:
        """__next__ dunder"""

class ManagedThing:
    def __init__(
        self, param1: typing.SupportsInt | typing.SupportsIndex, param2: typing.SupportsInt | typing.SupportsIndex
    ) -> None:
        """
        Initialise the wrapper, with the parameters and method to be called on the wrapped object.
        The wrapped object is NOT constructed until __enter__ is called.
        """
    def __call__(self) -> int:
        """
        Here you require at least one lambda to access the wrapped object and perform some operation on/with it.
        The object itself cannot be exposed to python as this will break RAII (you could bind the result of this call to a python variable
        and attempt access outside the context manager, invoking undefined behaviour - the memory will have been released).
        """
    def __enter__(self) -> typing.Any:
        """Enter context manager."""
    def __exit__(self, type_: typing.Any, value: typing.Any = "", traceback: typing.Any = None) -> None:
        """Exit context manager."""

class PrimeGenerator:
    """

    C++ implementation of a prime number generator.

    """
    def __init__(self) -> None: ...
    def __iter__(self) -> PrimeGenerator:
        """__iter__ dunder"""
    def __next__(self) -> int:
        """__next__ dunder"""

class PrimeRange:
    """

    C++ implementation of a prime number range.

    """
    def __init__(
        self, start: typing.SupportsInt | typing.SupportsIndex, length: typing.SupportsInt | typing.SupportsIndex
    ) -> None: ...
    def __iter__(self) -> PrimeRange:
        """__iter__ dunder"""
    def __next__(self) -> int:
        """__next__ dunder"""

class PrimeSieve:
    """

    C++ implementation of a prime number sieve.

    """
    def __init__(self, n: typing.SupportsInt | typing.SupportsIndex) -> None: ...
    def __iter__(self) -> PrimeSieve:
        """__iter__ dunder"""
    def __next__(self) -> int:
        """__next__ dunder"""

class Registry:
    """

    C++ implementation of a base class that accepts __init_subclass__ calls.
    Registry is a singleton that stores the types and params of subclasses, which can subsequently be accessed
    via the [] operator, an iterator, or by the dict-like items() method.

    """
    @staticmethod
    def __init_subclass__(*args, **kwargs):
        """(**kwargs) -> None"""
    def __getitem__(self, arg0: type) -> dict: ...
    def __init__(self) -> None: ...
    def __iter__(self) -> collections.abc.Iterator[typing.Any]: ...
    def items(self) -> collections.abc.Iterator[tuple[typing.Any, typing.Any]]: ...

class Triangle(Shape):
    def __init__(
        self,
        arg0: typing.SupportsFloat | typing.SupportsIndex,
        arg1: typing.SupportsFloat | typing.SupportsIndex,
        arg2: typing.SupportsFloat | typing.SupportsIndex,
    ) -> None:
        """Scalene triangle specified by the lengths of each side."""

def average_exectime(
    *, n: typing.SupportsInt | typing.SupportsIndex
) -> collections.abc.Callable[[collections.abc.Callable[_P, _R]], collections.abc.Callable[_P, tuple[float, _R]]]:
    """
    A parameterised decorator that averages execution time for a given number of repeats, implemented in C++.
    It returns a tuple containing the average exec time (in ms) and the function result
    """

def call_shape(arg0: Shape) -> str: ...
@typing.overload
def daxpy(
    a: typing.SupportsFloat | typing.SupportsIndex,
    x: typing.SupportsFloat | typing.SupportsIndex,
    y: typing.SupportsFloat | typing.SupportsIndex,
) -> float:
    """Perform a scalar double precision a-x-plus-y operation"""

@typing.overload
def daxpy(
    a: typing.Annotated[numpy.typing.ArrayLike, numpy.float64],
    x: typing.Annotated[numpy.typing.ArrayLike, numpy.float64],
    y: typing.Annotated[numpy.typing.ArrayLike, numpy.float64],
) -> typing.Any:
    """Perform a vectorised double precision a-x-plus-y operation"""

def exectime(function: collections.abc.Callable[_P, _R]) -> collections.abc.Callable[_P, tuple[float, _R]]:
    """A simple decorator that times execution, implemented in C++"""

def fib_recursive(n: typing.SupportsInt | typing.SupportsIndex) -> int:
    """Return nth value in fibonnacci sequence, computed recursively."""

def is_prime(n: typing.SupportsInt | typing.SupportsIndex) -> bool: ...
def nth_prime(n: typing.SupportsInt | typing.SupportsIndex) -> int: ...
def prime_factors(n: typing.SupportsInt | typing.SupportsIndex) -> list[int]: ...

ONE: CEnum  # value = <CEnum.ONE: 1>
TWO: CEnum  # value = <CEnum.TWO: 2>
