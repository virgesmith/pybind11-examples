#include "registry.h"
#include "decorators.h"
#include "fibonacci.h"
#include "collatz.h"
#include "managed_resource.h"
#include "primes.h"
#include "constants.h"
#include "animal.h"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

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
      C++ implementation base class that accepts __init_subclass__ calls.
    )""")
      .def(py::init<>())
      // workaround the lack of @classmethod in pybind11 by wrapping in a static that returns a lambda bound to the object
      .def_property_readonly_static("__init_subclass__", [](py::object& cls) {
        return py::cpp_function([cls](const py::kwargs& kwargs) { return Registry::init_subclass(cls, kwargs); });
      })
      .def_property_readonly_static("list", [](py::object&) { return Registry::registry; });

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
      // .def("todict", [](py::object) { return Constants::instance().dict(); })
      ;

    py::class_<Animal, PyAnimal>(m, "Animal")
      .def(py::init<>())
      .def("go", &Animal::go)
      .def("stop", &Animal::stop)
      .def("pop", &Animal::pop);

    py::class_<Dog, Animal>(m, "Dog")
      .def(py::init<>());

    m.def("call_animal", &call_animal);
}


