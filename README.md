# pybind11 examples

A python module `pybind11-examples` containing a C++ extension module, containing implementations of various constructs, including:

- generators, including `send`, `close`, and `throw` methods
- `__init_subclass__` - using it to register subclasses
- decorators, simple and parameterised
- a context manager
- some prime number stuff, for performance comparison aginst an equivalent rust implementation.
- polymorphism across C++ and python (subclassing C++ classes in python, calling python overrides through a C++ interface)

Most of the code originated in [this](http://github.com/virgesmith/poetry-pybind11-integration) project which was originally intended to demonstate how to make pybind11 work with poetry. The project evolved and became more about:
- how to implement certain python constructs in C++
- performance comparison between python modules implemented in [rust](https://github.com/virgesmith/poetry-rust-integration/) and C++

# usage

Build and install the package:

```sh
pip install -e .[dev]
```

Test:

```sh
pytest
```

Use, e.g.:

```py
from pybind11_examples import FibGenerator, Collatz

fg = FibGenerator()
print([next(fg) for _ in range(10)])

print(list(Collatz(19)))
```

## Type annotations

Type stubs are generated based on the signatures and docstrings in [module.cpp](src/module.cpp), using the `pybind11-stubgen` package directly on the C++ shared object, like so:

```
pybind11-stubgen _pybind11_examples
```

NB some manual corrections to the generated `__init__.pyi` are required.