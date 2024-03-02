// based on the documentation here: https://pybind11.readthedocs.io/en/stable/advanced/classes.html

#include <pybind11/pybind11.h>
#include <string>

class Animal {
public:
    virtual ~Animal() { }
    // pure virtual
    virtual std::string go(int n_times) const = 0;
    // virtual
    virtual std::string stop() const { return "Animal::stop"; }
    // nonvirtual
    std::string pop() const { return "Animal::pop"; }
};


class PyAnimal : public Animal {
public:
    /* Inherit the constructors */
    using Animal::Animal;
    // and assignment?
    using Animal::operator=;

    /* Trampolines (need one for each virtual function) */
    std::string go(int n_times) const override {
        PYBIND11_OVERRIDE_PURE(
            std::string, /* Return type */
            Animal,      /* Parent class */
            go,          /* Name of function in C++ (must match Python name) */
            n_times      /* Argument(s) */
        );
    }

    std::string stop() const override {
        PYBIND11_OVERRIDE(
            std::string,
            Animal,
            stop
        );
    }
};


class Dog : public Animal {
public:
    std::string go(int n_times) const override {
        std::string result;
        for (int i=0; i<n_times; ++i)
            result += "woof! ";
        return result;
    }

    // NB stop is not overridden
};

// access the (possibly python) object from C++ via a ref to its base
inline std::string call_animal(const Animal& animal)
{
    py::object obj = py::cast(&animal); // works for C++
    const std::string& name = obj.attr("__class__").attr("__name__").cast<std::string>();
    return name + " " + animal.go(3) + " " + animal.stop() + " " + animal.pop();
}
