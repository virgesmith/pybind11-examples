#pragma once

#include <pybind11/pybind11.h>

namespace py = pybind11;

class Registry
{
public:

    static void init_subclass(const py::type cls, const py::kwargs kwargs)
    {
        (*registry)[cls] = kwargs;
    }
    static py::dict* registry;
};
