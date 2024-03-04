from math import sqrt
import pytest
from pybind11_examples import Shape, Circle, Triangle, call_shape


def test_abstract_base() -> None:
    # ABC *can* be instantiated... (python subclasses need to call __init__)
    a = Shape()
    # ... but calling pure virtual functions will throw
    with pytest.raises(RuntimeError):
        a.area()
    # ...though others won't
    a.colour() # virtual
    a.dim() # nonvirtual


def test_cpp_subclasses() -> None:
    # Circle only implements the pure virtual methods
    c = Circle(1.0)
    assert c.area() == 3.14159
    assert c.perimeter() == 6.28318
    assert c.colour() == "Black"
    assert c.dim() == 2

    info = call_shape(c)
    assert "Circle" in info
    assert "2d" in info
    assert "A=3.14159" in info
    assert "L=6.28318" in info
    assert "Black" in info

    t = Triangle(3.0, 4.0, 5.0)
    assert t.area() == 6.0
    assert t.perimeter() == 12.0
    assert t.colour() == "Black"
    assert t.dim() == 2

    info = call_shape(t)
    assert "Triangle" in info
    assert "2d" in info
    assert "A=6.0" in info
    assert "L=12.0" in info
    assert "Black" in info


def test_py_subclasses() -> None:
    class Rectangle(Shape):
        def __init__(self, length: float, width: float) -> None:
            Shape.__init__(self)
            self.length = length
            self.width = width

        # implement the pure virtual method
        def area(self) -> float:
            return self.length * self.width

        # override the (virtual) default implementation
        def perimeter(self) -> float:
            return (self.length + self.width) * 2
        
        def colour(self) -> str:
            return "Green"

        # override the nonvirtual method (this will only affect direct calls from python)
        def dim(self) -> str:
            raise TypeError("nonvirtual impl not visible via Shape ref")
        
    class Square(Rectangle):
        def __init__(self, length: float) -> None:
            Rectangle.__init__(self, length, length)

        def colour(self) -> str:
            return "White"
        
    class EquilateralTriangle(Triangle):
        def __init__(self, length: float) -> None:
            Triangle.__init__(self, length, length, length)

        def colour(self) -> str:
            return "Red"

    # direct access
    r = Rectangle(3.0, 4.0)
    assert r.area() == 12.0
    assert r.perimeter() == 14.0
    with pytest.raises(TypeError):
        r.dim() # nonvirtual override is called

    s = Square(0.5)
    assert s.area() == 0.25
    assert s.perimeter() == 2.0
    with pytest.raises(TypeError):
        s.dim() # nonvirtual override (in Rectangle) is called


    # access via Base ref
    info = call_shape(r)
    assert "Rectangle" in info
    assert "2d" in info # nonvirtual override not called
    assert "A=12.0" in info
    assert "L=14.0" in info
    assert "Green" in info

    # access via Base ref
    info = call_shape(s)
    assert "Square" in info
    assert "2d" in info # nonvirtual override not called
    assert "A=0.25" in info
    assert "L=2.0" in info
    assert "White" in info

    e = EquilateralTriangle(1.0)
    assert e.area() == sqrt(0.1875)
    assert e.perimeter() == 3.0
    assert e.colour() == "Red"

    info = call_shape(e)
    assert "EquilateralTriangle" in info
    assert "2d" in info
    assert "A=0.433" in info
    assert "L=3.0" in info
    assert "Red" in info # https://pybind11.readthedocs.io/en/stable/advanced/classes.html#combining-virtual-functions-and-inheritance

if __name__ == "__main__":
    test_abstract_base()
    test_cpp_subclasses()
    test_py_subclasses()