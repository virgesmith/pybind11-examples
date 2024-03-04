// based on the documentation here: https://pybind11.readthedocs.io/en/stable/advanced/classes.html
#include "print.h"

#include <pybind11/pybind11.h>
#include <string>
#include <cmath>

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
  // Inherit the constructors. why, Shape is abstract? 
  // using Shape::Shape;

  /* Trampolines (need one for each virtual function) */
  double area() const override {
    PYBIND11_OVERRIDE_PURE(
      double, /* Return type */
      Shape,  /* Parent class */
      area,   /* Name of function in C++ (must match Python name) */
              /*...       Argument(s) */
    );
  }

  double perimeter() const override {
    PYBIND11_OVERRIDE_PURE(double, Shape, perimeter,);
  }

  std::string colour() const override {
    PYBIND11_OVERRIDE(std::string, Shape, colour,);
  }
};

class Circle : public Shape {
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

class Triangle: public Shape {
public:
  Triangle(double a, double b, double c) : m_a(a), m_b(b), m_c(c) {}

  double area() const override {
    double p2 = perimeter() / 2;
    return sqrt(p2 * (p2 - m_a) * (p2 - m_b) * (p2 - m_c));
  }

  double perimeter() const override {
    return m_a + m_b + m_c;
  }

private:
  double m_a;
  double m_b;
  double m_c;
};

class PyTriangle : public Triangle {
public:
  using Triangle::Triangle; // Inherit constructors
  double area() const override { PYBIND11_OVERRIDE(double, Triangle, area,); }

  double perimeter() const override { PYBIND11_OVERRIDE(double, Triangle, perimeter,); }

  std::string colour() const override { PYBIND11_OVERRIDE(std::string, Triangle, colour,); }
};


// access the (possibly python) object from C++ via a ref to its base
inline std::string call_shape(const Shape &shape)
{
  py::object obj = py::cast(&shape); // works for C++
  const std::string &name = obj.attr("__class__").attr("__name__").cast<std::string>();
  return "<%% %%d A=%% L=%% %%>"_s % name % shape.dim() % shape.area() % shape.perimeter() % shape.colour();
}
