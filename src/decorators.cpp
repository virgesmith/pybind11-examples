#include "decorators.h"
#include "timer.h"


py::cpp_function exectime(py::function f)
{
    return [=](py::args args, const py::kwargs& kwargs) -> py::object {
        Timer t;
        py::object result = f(*args, **kwargs);
        return py::make_tuple(t.elapsed_ms(), result);
    };
}


py::cpp_function average_exectime(size_t n)
{
    return [=](py::function f) -> py::cpp_function {
        return [=](py::args args, const py::kwargs& kwargs) -> py::object {
            Timer t;
            py::object result;
            for (size_t i = 0; i < n; ++i)
                result = f(*args, **kwargs);
            return py::make_tuple(t.elapsed_ms() / n, result);
        };
    };
}
