// based on the documentation here: https://pybind11.readthedocs.io/en/stable/advanced/classes.html
#include "print.h"

#include <pybind11/pybind11.h>
#include <string>

class Shape
{
public:
  virtual ~Shape() {}
  // pure virtual
  virtual double area() const = 0;
  virtual double perimeter() const = 0;
  // virtual
  virtual std::string colour() const { return "Black"; }
  // nonvirtual
  int dim() const { return 2; }
};

class PyShape : public Shape
{
public:
  /* Inherit the constructors. why? */
  using Shape::Shape;
  // and assignment?
  using Shape::operator=;

  /* Trampolines (need one for each virtual function) */
  double area() const override {
    PYBIND11_OVERRIDE_PURE(
      double, /* Return type */
      Shape,  /* Parent class */
      area    /* Name of function in C++ (must match Python name) */
              /*...       Argument(s) */
    );
  }

  double perimeter() const override {
    PYBIND11_OVERRIDE_PURE(double, Shape, perimeter);
  }

  std::string colour() const override {
    PYBIND11_OVERRIDE(std::string, Shape, colour);
  }
};

class Circle : public Shape
{
public:
  Circle(double r) : m_r(r) {}

  double area() const override {
    return 3.14159 * m_r * m_r;
  }

  double perimeter() const override {
    return 2 * 3.14159 * m_r;
  }

private:
  double m_r;
};

// access the (possibly python) object from C++ via a ref to its base
inline std::string call_shape(const Shape &shape)
{
  py::object obj = py::cast(&shape); // works for C++
  const std::string &name = obj.attr("__class__").attr("__name__").cast<std::string>();
  return "<%% %%d A=%% L=%% %%>"_s % name % shape.dim() % shape.area() % shape.perimeter() % shape.colour();
}
