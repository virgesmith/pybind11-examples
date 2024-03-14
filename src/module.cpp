#include "registry.h"
#include "decorators.h"
#include "fibonacci.h"
#include "collatz.h"
#include "managed_resource.h"
#include "primes.h"
#include "constants.h"
#include "shape.h"
#include "enums.h"
#include "vectorised.h"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

namespace py = pybind11;

using namespace py::literals;


PYBIND11_MODULE(_pybind11_examples, m)
{
    m.doc() = R"""(
      Pybind11 example plugin
      -----------------------
      .. currentmodule:: _pybind11_examples
      .. autosummary::
          :toctree: _generate
    )""";

    m.def("fib_recursive", &fib_recursive, "n"_a, R"""(
      Return nth value in fibonnacci sequence, computed recursively.
      )""");

    py::class_<FibGenerator>(m, "FibGenerator", R"""(
      C++ implementation of a Fibonacci sequence generator.
    )""")
      .def(py::init<>())
      .def("__iter__", &FibGenerator::iter, "__iter__ dunder")
      .def("__next__", &FibGenerator::next, "__next__ dunder")
      ;

    py::class_<Collatz>(m, "Collatz", R"""(
      C++ implementation of a Collatz sequence generator.
      )""")
      .def(py::init<uint64_t>(), "n"_a)
      .def("__iter__", &Collatz::iter, "return iter")
      .def("__next__", &Collatz::next, "return next item")
      .def("send", &Collatz::send, "generator send")
      .def("send", py::overload_cast<>(&Collatz::next), "generator send (equivalent to next()")
      .def("close", &Collatz::close, "generator close")
      .def("throw", &Collatz::throw_, "type"_a, "value"_a = py::str(), "traceback"_a = py::none(), "generator throw")
      .def("throw", py::overload_cast<>(&Collatz::throw_default), "generator throw default")
      ;

    py::class_<Registry>(m, "Registry", R"""(
      C++ implementation of a base class that accepts __init_subclass__ calls.
      Registry is a singleton that stores the types and params of subclasses, which can subsequently be accessed
      via the [] operator, an iterator, or by the dict-like items() method.
    )""")
      .def(py::init<>())
      // workaround the lack of @classmethod in pybind11 by wrapping in a static that returns a lambda bound to the object
      .def_property_readonly_static("__init_subclass__", [](py::object& cls) {
        return py::cpp_function([cls](const py::kwargs& kwargs) { return Registry::init_subclass(cls, kwargs); });
      })
      .def("__getitem__",
        [](py::object, py::type key) -> py::dict {
          return (*Registry::registry)[key];
        })
      .def("__iter__",
        [](py::object) {
          return py::make_key_iterator(Registry::registry->begin(), Registry::registry->end());
        },
        py::keep_alive<0, 1>())
      .def("items",
        [](py::object) {
          return py::make_iterator(Registry::registry->begin(), Registry::registry->end());
        },
        py::keep_alive<0, 1>())
      ;

    // decorator
    m.def("exectime", &exectime, R"""(
      A simple decorator that times execution, implemented in C++
      )""")
      .def("average_exectime", &average_exectime, py::kw_only(), py::arg("n"), R"""(
      A parameterised decorator that averages execution time for a given number of repeats, implemented in C++
      )""");

    //
    py::class_<ManagedResource<Thing, int, int>>(m, "ManagedThing")
      .def(py::init<int, int>(), "param1"_a, "param2"_a)
      .def("__call__", [](const ManagedResource<Thing, int, int>& wrapper) { return wrapper().do_the_thing(); }, R"""(
        Here you require at least one lambda to access the wrapped object and perform some operation on/with it.
        The object itself cannot be exposed to python as this will break RAII (you could bind the result of this call to a python variable
        and attempt access outside the context manager, invoking undefined behaviour - the memory will have been released).
      )""")
      .def("__enter__", &ManagedResource<Thing, int, int>::enter, "Enter context manager.")
      .def("__exit__", &ManagedResource<Thing, int, int>::exit, "type"_a, "value"_a = py::str(), "traceback"_a = py::none(), "Exit context manager.");

    py::class_<PrimeSieve>(m, "PrimeSieve", R"""(
      C++ implementation of a prime number sieve.
      )""")
      .def(py::init<size_t>(), "n"_a)
      .def("__iter__", &PrimeSieve::iter, "__iter__ dunder")
      .def("__next__", &PrimeSieve::next, "__next__ dunder")
      ;

    py::class_<PrimeGenerator>(m, "PrimeGenerator", R"""(
      C++ implementation of a prime number generator.
      )""")
      .def(py::init<>())
      .def("__iter__", &PrimeGenerator::iter, "__iter__ dunder")
      .def("__next__", &PrimeGenerator::next, "__next__ dunder")
      ;

    py::class_<PrimeRange>(m, "PrimeRange", R"""(
      C++ implementation of a prime number generator.
      )""")
      .def(py::init<size_t, size_t>(), "start"_a, "length"_a)
      .def("__iter__", &PrimeRange::iter, "__iter__ dunder")
      .def("__next__", &PrimeRange::next, "__next__ dunder")
      ;

    m.def("is_prime", &is_prime_py, "n"_a)
      .def("nth_prime", &nth_prime_py, "n"_a)
      .def("prime_factors", &prime_factors, "n"_a);

    // py::dynamic_attr() adds __dict__ but cant override: AttributeError: attribute '__dict__' of 'type' objects is not writable
    py::class_<Constants, std::unique_ptr<Constants, py::nodelete>>(m, "Constants", R"""(
      Singleton wrapper for immutable values
    )""")
      .def(py::init(
        [](py::kwargs kwargs) {
          return std::unique_ptr<Constants, py::nodelete>(&Constants::instance(kwargs));
        }))
      .def("__repr__",
        [](py::object) {
          return Constants::instance().repr();
        })
      .def("__getattr__",
        [](py::object, const std::string& name) {
          return Constants::instance().get(name);
        })
      .def("__setattr__",
        [](py::object, const std::string& name, Constants::mapped_type value) {
          return Constants::instance().set(name, value);
        })
      .def("__delattr__",
        [](py::object, const std::string& name) {
          throw py::attribute_error("can't delete immutable: %%"s % name);
        })
      .def("__iter__",
        [](py::object) {
          return py::make_iterator(Constants::instance().keys_begin(), Constants::instance().keys_end());
        },
        py::keep_alive<0, 1>())
      .def("items",
        [](py::object) {
          return py::make_iterator(Constants::instance().store_begin(), Constants::instance().store_end());
        },
        py::keep_alive<0, 1>())
      ;

    // cross-language polymorphism
    // ABC
    py::class_<Shape, PyShape>(m, "Shape")
      .def(py::init<>()) // required see https://pybind11.readthedocs.io/en/stable/advanced/classes.html
      .def("area", &Shape::area)  // pure virtual
      .def("perimeter", &Shape::perimeter)  // pure virtual
      .def("colour", &Shape::colour)  // virtual
      .def("dim", &Shape::dim);  // nonvirtual

    // C++ subclasses, only need to register the constructor
    py::class_<Circle, Shape>(m, "Circle")
      .def(py::init<double>());

    py::class_<Triangle, Shape, PyTriangle>(m, "Triangle")
      .def(py::init<double, double, double>());

    m.def("call_shape", &call_shape);

    py::enum_<CEnum>(m, "CEnum")
      .value("ONE", CEnum::ONE)
      .value("TWO", CEnum::TWO)
      .export_values() // pointlessly replicates scoping issues with C enums
    ;
    // not scoped and comparison with int is valid - python replicates this
    static_assert(ONE == 1);

    py::enum_<CppEnum>(m, "CppEnum")
      .value("THREE", CppEnum::THREE)
      .value("FOUR", CppEnum::FOUR)
    ;
    // scoped and comparison with int is invalid - python replicates this
    // static_assert(CppEnum::THREE == 3);

    // auto-vectorised function. As the vectorised version is much slower when called with scalar arguments an overload is provided for this case
    m.def("daxpy", py::overload_cast<double, double, double>(daxpy), "Perform a scalar double precision a-x-plus-y operation", "a"_a, "x"_a, "y"_a)
     .def("daxpy", py::vectorize(daxpy), "Perform a vectorised double precision a-x-plus-y operation", "a"_a, "x"_a, "y"_a);
}