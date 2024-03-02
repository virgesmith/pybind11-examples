#pragma once

#include <pybind11/pybind11.h>

namespace py = pybind11;


py::cpp_function exectime(py::function f);

py::cpp_function average_exectime(size_t n);
