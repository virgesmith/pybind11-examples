#pragma once

#include <pybind11/pybind11.h>

#include <memory>
#include <functional>

namespace py = pybind11;


// Wrapper enabling an RAII resource to use used (solely) within a context manager.
// Resource construction is deferred to the enter function, and release brought forward to the exit function
// see my answer here https://stackoverflow.com/questions/55452762/pybind11-destructor-not-invoked/74656071#74656071
template<typename T, typename... Args>
class ManagedResource final
{
    // Note: T must NOT have python bindings, otherwise it's possible for python to have a
    // dangling reference to this object outside the context manager invoking undefined beahviour
    typedef T wrapped_type;
public:
    // use a lambda to store args for deferred construction entering the with block
    ManagedResource(wrapped_type::call_type f, Args... args) : call_function(f), initialiser{[args...]() -> std::unique_ptr<wrapped_type> { return std::make_unique<wrapped_type>(args...); }} { }

    ~ManagedResource() { }

    ManagedResource(const ManagedResource&) = delete;
    ManagedResource& operator=(const ManagedResource&) = delete;
    ManagedResource(ManagedResource&&) = default;
    ManagedResource& operator=(ManagedResource&&) = default;

    auto operator()() const
    {
        if (!resource)
        {
            throw std::runtime_error("ManagedResource should only be used within a context manager");
        }
        return std::bind(call_function, *resource)();
    }

    py::object enter()
    {
        // acquire resources
        resource = initialiser();
        return py::cast(this);
    }

    void exit(py::handle type, py::handle value, py::handle traceback)
    {
        // release resources
        resource.reset();
    }

private:
    wrapped_type::call_type call_function; 
    std::function<std::unique_ptr<wrapped_type>()> initialiser;
    std::unique_ptr<wrapped_type> resource;
};

// TODO general macro
#define CONTEXT_MANAGED(TYPE, METHOD, ...)                                                        \
  py::class_<ManagedResource<TYPE, __VA_ARGS__>>(m, "Managed"#TYPE)                               \
    .def(py::init([](int a, int b) { /* <--PROBLEM how to generalise args--v */                   \
        return std::move(ManagedResource<TYPE, __VA_ARGS__>(&TYPE::METHOD, a, b));                \
      }))                                                                                         \
    .def("__call__", [](const ManagedResource<TYPE, __VA_ARGS__>& wrapper) { return wrapper(); }) \
    .def("__enter__", &ManagedResource<TYPE, __VA_ARGS__>::enter)                                 \
    .def("__exit__", &ManagedResource<TYPE, __VA_ARGS__>::exit);



// example of resource to be managed
struct Thing final {
  Thing(int i, int j) : i(i), j(j)
  {
    py::print("constructed a Thing");
  }

  ~Thing()
  {
    py::print("destructing a Thing");
  }

  int do_the_thing() const
  {
    return i * j;
  }

  typedef decltype(&Thing::do_the_thing) call_type;

private:
  int i, j;
};

