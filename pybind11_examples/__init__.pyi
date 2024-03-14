"""

      Pybind11 example plugin
      -----------------------
      .. currentmodule:: _pybind11_examples
      .. autosummary::
          :toctree: _generate
    
"""
from __future__ import annotations
import numpy
import typing
__all__ = ['CEnum', 'Circle', 'Collatz', 'Constants', 'CppEnum', 'FibGenerator', 'ManagedThing', 'ONE', 'PrimeGenerator', 'PrimeRange', 'PrimeSieve', 'Registry', 'Shape', 'TWO', 'Triangle', 'average_exectime', 'call_shape', 'daxpy', 'exectime', 'fib_recursive', 'is_prime', 'nth_prime', 'prime_factors']
class CEnum:
    """
    Members:
    
      ONE
    
      TWO
    """
    ONE: typing.ClassVar[CEnum]  # value = <CEnum.ONE: 1>
    TWO: typing.ClassVar[CEnum]  # value = <CEnum.TWO: 2>
    __members__: typing.ClassVar[dict[str, CEnum]]  # value = {'ONE': <CEnum.ONE: 1>, 'TWO': <CEnum.TWO: 2>}
    def __eq__(self, other: typing.Any) -> bool:
        ...
    def __getstate__(self) -> int:
        ...
    def __hash__(self) -> int:
        ...
    def __index__(self) -> int:
        ...
    def __init__(self, value: int) -> None:
        ...
    def __int__(self) -> int:
        ...
    def __ne__(self, other: typing.Any) -> bool:
        ...
    def __repr__(self) -> str:
        ...
    def __setstate__(self, state: int) -> None:
        ...
    def __str__(self) -> str:
        ...
    @property
    def name(self) -> str:
        ...
    @property
    def value(self) -> int:
        ...
class Circle(Shape):
    def __init__(self, radius: float) -> None:
        ...
class Collatz:
    """
    
          C++ implementation of a Collatz sequence generator.
          
    """
    def __init__(self, n: int) -> None:
        ...
    def __iter__(self) -> Collatz:
        """
        return iter
        """
    def __next__(self) -> int:
        """
        return next item
        """
    def close(self) -> None:
        """
        generator close
        """
    @typing.overload
    def send(self, arg0: int) -> int:
        """
        generator send
        """
    @typing.overload
    def send(self) -> int:
        """
        generator send (equivalent to next()
        """
    @typing.overload
    def throw(self, type: type, value: str = '', traceback: typing.Any = None) -> None:
        """
        generator throw
        """
    @typing.overload
    def throw(self) -> None:
        """
        generator throw default
        """
class Constants:
    """
    
          Singleton wrapper for immutable values
        
    """
    def __delattr__(self, arg0: str) -> None:
        ...
    def __getattr__(self, arg0: str) -> typing.Any:
        ...
    def __init__(self, **kwargs) -> None:
        ...
    def __iter__(self) -> typing.Iterator:
        ...
    def __repr__(self) -> str:
        ...
    def __setattr__(self, arg0: str, arg1: typing.Any) -> None:
        ...
    def items(self) -> typing.Iterator:
        ...
class CppEnum:
    """
    Members:
    
      THREE
    
      FOUR
    """
    FOUR: typing.ClassVar[CppEnum]  # value = <CppEnum.FOUR: 4>
    THREE: typing.ClassVar[CppEnum]  # value = <CppEnum.THREE: 3>
    __members__: typing.ClassVar[dict[str, CppEnum]]  # value = {'THREE': <CppEnum.THREE: 3>, 'FOUR': <CppEnum.FOUR: 4>}
    def __eq__(self, other: typing.Any) -> bool:
        ...
    def __getstate__(self) -> int:
        ...
    def __hash__(self) -> int:
        ...
    def __index__(self) -> int:
        ...
    def __init__(self, value: int) -> None:
        ...
    def __int__(self) -> int:
        ...
    def __ne__(self, other: typing.Any) -> bool:
        ...
    def __repr__(self) -> str:
        ...
    def __setstate__(self, state: int) -> None:
        ...
    def __str__(self) -> str:
        ...
    @property
    def name(self) -> str:
        ...
    @property
    def value(self) -> int:
        ...
class FibGenerator:
    """
    
          C++ implementation of a Fibonacci sequence generator.
        
    """
    def __init__(self) -> None:
        ...
    def __iter__(self) -> FibGenerator:
        """
        __iter__ dunder
        """
    def __next__(self) -> int:
        """
        __next__ dunder
        """
class ManagedThing:
    def __call__(self) -> int:
        """
                Here you require at least one lambda to access the wrapped object and perform some operation on/with it.
                The object itself cannot be exposed to python as this will break RAII (you could bind the result of this call to a python variable
                and attempt access outside the context manager, invoking undefined behaviour - the memory will have been released).
        """
    def __enter__(self) -> typing.Any:
        """
        Enter context manager.
        """
    def __exit__(self, type: typing.Any, value: typing.Any = '', traceback: typing.Any = None) -> None:
        """
        Exit context manager.
        """
    def __init__(self, param1: int, param2: int) -> None:
        ...
class PrimeGenerator:
    """
    
          C++ implementation of a prime number generator.
          
    """
    def __init__(self) -> None:
        ...
    def __iter__(self) -> PrimeGenerator:
        """
        __iter__ dunder
        """
    def __next__(self) -> int:
        """
        __next__ dunder
        """
class PrimeRange:
    """
    
          C++ implementation of a prime number generator.
          
    """
    def __init__(self, start: int, length: int) -> None:
        ...
    def __iter__(self) -> PrimeRange:
        """
        __iter__ dunder
        """
    def __next__(self) -> int:
        """
        __next__ dunder
        """
class PrimeSieve:
    """
    
          C++ implementation of a prime number sieve.
          
    """
    def __init__(self, n: int) -> None:
        ...
    def __iter__(self) -> PrimeSieve:
        """
        __iter__ dunder
        """
    def __next__(self) -> int:
        """
        __next__ dunder
        """
class Registry:
    """
    
          C++ implementation of a base class that accepts __init_subclass__ calls.
          Registry is a singleton that stores the types and params of subclasses, which can subsequently be accessed
          via the [] operator, an iterator, or by the dict-like items() method.
        
    """
    @staticmethod
    def __init_subclass__(*args, **kwargs):
        """
        (**kwargs) -> None
        """
    def __getitem__(self, arg0: type) -> dict:
        ...
    def __init__(self) -> None:
        ...
    def __iter__(self) -> typing.Iterator:
        ...
    def items(self) -> typing.Iterator:
        ...
class Shape:
    def __init__(self) -> None:
        ...
    def area(self) -> float:
        ...
    def colour(self) -> str:
        ...
    def dim(self) -> int:
        ...
    def perimeter(self) -> float:
        ...
class Triangle(Shape):
    def __init__(self, arg0: float, arg1: float, arg2: float) -> None:
        """
        Scalene triangle specified by the lengths of each side.
        """
def average_exectime(*, n: int) -> cpp_function:
    """
          A parameterised decorator that averages execution time for a given number of repeats, implemented in C++
    """
def call_shape(arg0: Shape) -> str:
    ...
@typing.overload
def daxpy(a: float, x: float, y: float) -> float:
    """
    Perform a scalar double precision a-x-plus-y operation
    """
@typing.overload
def daxpy(a: numpy.ndarray[numpy.float64], x: numpy.ndarray[numpy.float64], y: numpy.ndarray[numpy.float64]) -> typing.Any:
    """
    Perform a vectorised double precision a-x-plus-y operation
    """
def exectime(arg0: typing.Callable) -> cpp_function:
    """
          A simple decorator that times execution, implemented in C++
    """
def fib_recursive(n: int) -> int:
    """
          Return nth value in fibonnacci sequence, computed recursively.
    """
def is_prime(n: int) -> bool:
    ...
def nth_prime(n: int) -> int:
    ...
def prime_factors(n: int) -> list[int]:
    ...
ONE: CEnum  # value = <CEnum.ONE: 1>
TWO: CEnum  # value = <CEnum.TWO: 2>
